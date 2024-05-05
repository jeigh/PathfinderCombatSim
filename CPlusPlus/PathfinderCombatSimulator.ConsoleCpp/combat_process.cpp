#include "types.h"

using namespace pathfinder_combat_simulator;

using std::make_shared;
using std::logic_error;

int combat_algorithm::roll_initiative(nullable<mobile_object> const actor) const
{
	return actor->perception_skill_check_modifier + _rng->roll(20);
}

vector<nullable<tuple<nullable<mobile_object>, int>>> combat_algorithm::roll_and_order_by_initiative(vector<nullable<combat_team>> const groups) const
{
	auto initiative_table = vector<nullable<tuple<nullable<mobile_object>, int>>>();

	for (nullable<combat_team> group : groups)
	{
		for (nullable<mobile_object> combatant : group->combatants)
		{
			int rolled = roll_initiative(combatant);
			auto back_pushable = make_shared<tuple<nullable<mobile_object>, int>>(combatant, rolled);
			initiative_table.push_back(back_pushable);
		}
	}

	return initiative_table;
}

bool combat_algorithm::is_combat_still_active(vector<nullable<combat_team>> const combat_groups) const
{
	auto alive_count = 0;
	for (nullable<combat_team> combat_group : combat_groups)
	{
		if (still_lives(combat_group)) alive_count++;
	}
	return (alive_count > 1);
}

nullable< unordered_map<damage_type, int>> combat_algorithm::roll_damage(vector<damage_effect> const damage_effects) const
{
	auto returnable = make_shared<unordered_map<damage_type, int>>();

	for (damage_effect damage_effect : damage_effects)
	{
		int amount_to_add = _rng->add_rolls(damage_effect.damage_dice);
		damage_type key = damage_effect._damage_type;

		if (returnable->contains(damage_effect._damage_type))
			returnable->insert_or_assign(key, returnable->at(key) + amount_to_add);
		else
			returnable->insert({ key, amount_to_add });
	}

	return returnable;
}

int combat_algorithm::roll_to_hit(int const number_of_previous_attacks_this_turn, int const attack_modifier) const
{
	return _rng->roll(20) + attack_modifier - (number_of_previous_attacks_this_turn * 5);
}

void combat_algorithm::process_battle_until_only_one_team_is_concious(const nullable<battle> the_battle) const
{
	if (the_battle->combat_teams.size() > 2) throw logic_error("Only two or fewer combat groups are supported.");
	auto sorted_initiative = roll_and_order_by_initiative(the_battle->combat_teams);
	int roundId = 1;
	while (is_combat_still_active(the_battle->combat_teams))
	{
		_ui->round_starts(the_battle, roundId);
		auto previous_attack_count_for_mob_this_round = vector<tuple<nullable<mobile_object>, int>>();
		for (const nullable<tuple<nullable<mobile_object>, int>>& kvp : sorted_initiative)
		{
			nullable<mobile_object> mob = std::get<0>(*kvp);
			auto initiative_roll = std::get<1>(*kvp);

			unordered_map<nullable<mobile_object>, int> mapified;
			for (tuple<nullable<mobile_object>, int> the_tuple : previous_attack_count_for_mob_this_round)
				mapified.insert({ std::get<0>(the_tuple), std::get<1>(the_tuple) });

			if (has_available_action_this_turn(mob)) process_action(the_battle, 1, mob, mapified);
			if (has_available_action_this_turn(mob)) process_action(the_battle,2, mob, mapified);
			if (has_available_action_this_turn(mob)) process_action(the_battle, 3, mob, mapified);
		}
		_ui->round_ends(the_battle, roundId);
		roundId++;
	}
}


bool combat_algorithm::still_lives(nullable<combat_team> const acg) const
{
	for (nullable<mobile_object> mob : acg->combatants)
	{
		if (!mob->is_dead()) return true;
	}
	return false;
}

nullable<int> combat_algorithm::calculate_modified_ac(nullable<mobile_object> const victim) const
{
	nullable<int> modified_armor_class = make_shared<int>(victim->unmodified_current_armor_class);

	if (victim->is_unconcious()) *modified_armor_class -= 4;

	return modified_armor_class;
}

nullable<attack_results> combat_algorithm::attack(
	nullable<mobile_object> const attacker,
	nullable<mobile_object> const victim,
	int const number_of_previous_attacks_made_this_turn) const
{
	if (attacker->default_attack != nullptr)
	{
		const int attack_value = roll_to_hit(number_of_previous_attacks_made_this_turn,
			attacker->default_attack->attack_modifier_);

		nullable<int> modified_armor_class = calculate_modified_ac(victim);

		if (attack_value > *modified_armor_class)
		{
			auto returnable = make_shared<attack_results>();
			auto damage_roll = roll_damage(attacker->default_attack->damage_effects_);

			//todo: update to work with more than just physical damage...
			if (damage_roll->at(physical))
			{
				auto damage = damage_roll->at(physical);

				if (returnable != nullptr)
				{
					returnable->attacking_mob = attacker;
					returnable->target_mob = victim;
					returnable->damage_delivered = damage;
				}
			}

			//todo: apply saving throw feature?
			return returnable;
		}
	}
	return nullptr;
}

void combat_algorithm::process_action(
	nullable<battle> the_battle,
	int const action_number,
	nullable<mobile_object> acting_mob,
	unordered_map<nullable<mobile_object>, int> dictionary_of_previous_attack_counts) const
{
	nullable<mobile_object> target_mob = _ai->get_target_for(acting_mob, the_battle->combat_teams);
	if (target_mob != nullptr && !acting_mob->is_dead() && !acting_mob->is_unconcious())
	{
		nullable<mobile_object> victim = target_mob;

		int prev_attacks = 0;
		if (dictionary_of_previous_attack_counts.contains(acting_mob))
			prev_attacks = dictionary_of_previous_attack_counts.at(acting_mob);

		auto results = attack(acting_mob, victim, prev_attacks);
		if (dictionary_of_previous_attack_counts.contains(acting_mob))
			dictionary_of_previous_attack_counts.at(acting_mob) = prev_attacks++;

		else if (results != nullptr && results->target_mob != nullptr)
		{
			if (results->damage_delivered <= 0)
			{
				_ui->attack_misses(the_battle, acting_mob, victim);
				return;
			}
			const int previous_hit_points = results->target_mob->current_hit_points;
			const int updated_hit_points = previous_hit_points - results->damage_delivered;
			results->target_mob->current_hit_points = updated_hit_points;

			_ui->receive_damage(the_battle, results);

			if (previous_hit_points > -10 && results->target_mob->is_dead())
				_ui->die(the_battle, results->target_mob);

			if (previous_hit_points > 0 && results->target_mob->is_unconcious())
				_ui->knock_out(the_battle, results->target_mob);
		}
	}
}

bool combat_algorithm::has_available_action_this_turn(nullable<mobile_object> const mob) const
{
	return true;
}