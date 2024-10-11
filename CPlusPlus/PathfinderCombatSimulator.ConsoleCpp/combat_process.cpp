#include <algorithm>



#include "types.h"

using namespace pathfinder_combat_simulator;

using std::make_shared;
using std::logic_error;

int combat_process::roll_initiative(shared_ptr<mobile_object> const actor) const
{
	return actor->perception_skill_check_modifier + _rng->roll(20);
}

bool combat_process::is_combat_still_active(vector<shared_ptr<combat_team>> const combat_groups) const
{
	auto alive_count = 0;
	for (shared_ptr<combat_team> combat_group : combat_groups)
	{
		if (still_lives(combat_group)) alive_count++;
	}
	return (alive_count > 1);
}

shared_ptr< unordered_map<damage_type, int>> combat_process::roll_damage(vector<damage_effect> const damage_effects) const
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

[[nodiscard]] int combat_process::apply_attack_modifiers(shared_ptr<mobile_object> mob, int d20) const
{
	return d20 + mob->default_attack->attack_modifier_ - (mob->number_of_previous_attacks_made_this_turn * 5);
}

void combat_process::process_battle_until_only_one_team_is_concious(const shared_ptr<battle> the_battle)
{

	int roundId = 1;
	while (is_combat_still_active(the_battle->combat_teams))
	{
		auto table = roll_and_order_by_initiative(the_battle);

		_ui->round_starts(the_battle, roundId);
		for (std::shared_ptr<mobile_object> mob : table->get_ordered_mobs())
		{
			if (has_available_action_this_turn(mob)) process_action(the_battle, 1, mob);
			if (has_available_action_this_turn(mob)) process_action(the_battle, 2, mob);
			if (has_available_action_this_turn(mob)) process_action(the_battle, 3, mob);
		}
		_ui->round_ends(the_battle, roundId);
		the_battle->round_ends();
		roundId++;
	}
}

[[nodiscard]] shared_ptr<initiative_table> combat_process::roll_and_order_by_initiative(shared_ptr<battle> the_battle) const
{
	auto table = make_shared<initiative_table>();
	for (shared_ptr<combat_team> const combat_team : the_battle->combat_teams)
	{
		for (shared_ptr<mobile_object> const combatant : combat_team->combatants)
		{
			int roll_value = roll_initiative(combatant);
			table->add_roll_by_mob(combatant, roll_value);
		}
	}
	return table;
}


bool combat_process::still_lives(shared_ptr<combat_team> const acg) const
{
	for (shared_ptr<mobile_object> mob : acg->combatants)
	{
		if (!mob->is_dead()) return true;
	}
	return false;
}

auto combat_process::calculate_modified_ac(shared_ptr<mobile_object> const victim) const -> shared_ptr<int>
{
	shared_ptr<int> modified_armor_class = make_shared<int>(victim->unmodified_current_armor_class);

	if (victim->is_unconcious()) *modified_armor_class -= 4;

	return modified_armor_class;
}

auto combat_process::attack(
	shared_ptr<battle> the_battle,
	shared_ptr<mobile_object> const attacker,
	shared_ptr<mobile_object> const victim)  -> shared_ptr<attack_results>
{
	if (attacker->default_attack != nullptr)
	{

		
		int const unmodified_attack_roll = _rng->roll(20);
		int const attack_value = apply_attack_modifiers(attacker, unmodified_attack_roll);

		shared_ptr<int> modified_armor_class = calculate_modified_ac(victim);

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
		
		collect_data_payload(attacker, victim, unmodified_attack_roll);

		attacker->number_of_previous_attacks_made_this_turn += 1;
	}
	return nullptr;
}

void pathfinder_combat_simulator::combat_process::collect_data_payload(
	const shared_ptr<mobile_object>& attacker, 
	const shared_ptr<mobile_object>& victim,
	const int unmodified_attack_roll) 
{
	auto addable = std::make_unique<data_collection_payload>();

	addable->set_attacker(attacker);
	addable->set_defender(victim);

	addable->attackers_momentary_attack_bonus = attacker->default_attack->attack_modifier_;
	addable->defenders_momentary_ac = *calculate_modified_ac(victim);
	addable->unmodified_attack_roll = unmodified_attack_roll;

	auto dal = std::make_unique<data_access>();
	
	dal->insert_payload(std::move(addable));
}

void combat_process::process_action(
	shared_ptr<battle> const the_battle,
	int const action_number,
	shared_ptr<mobile_object> acting_mob) 
{
	shared_ptr<mobile_object> target_mob = _ai->get_target_for(acting_mob, the_battle->combat_teams);
	if (target_mob != nullptr && !acting_mob->is_dead() && !acting_mob->is_unconcious())
	{
		shared_ptr<mobile_object> victim = target_mob;
		auto results = attack(the_battle, acting_mob, victim);

		if (results != nullptr && results->target_mob != nullptr)
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

bool combat_process::has_available_action_this_turn(shared_ptr<mobile_object> const mob) const
{
	//todo: correct this logic
	return true;
}
