
#include <algorithm>



#include "types.h"

using namespace pathfinder_combat_simulator;

using std::make_shared;
using std::logic_error;


auto attack_process::do_attack(
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

shared_ptr< unordered_map<damage_type, int>> attack_process::roll_damage(vector<damage_effect> const damage_effects) const
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

[[nodiscard]] int attack_process::apply_attack_modifiers(shared_ptr<mobile_object> mob, int d20) const
{
	return d20 + mob->default_attack->attack_modifier_ - (mob->number_of_previous_attacks_made_this_turn * 5);
}


auto attack_process::calculate_modified_ac(shared_ptr<mobile_object> const victim) const -> shared_ptr<int>
{
	shared_ptr<int> modified_armor_class = make_shared<int>(victim->unmodified_current_armor_class);

	if (victim->is_unconcious()) *modified_armor_class -= 4;

	return modified_armor_class;
}

void pathfinder_combat_simulator::attack_process::collect_data_payload(
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
}
