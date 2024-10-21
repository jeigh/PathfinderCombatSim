
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
		int unmodified_attack_roll = _rng->roll(20);
		int attack_value = apply_attack_modifiers(attacker, unmodified_attack_roll);

		shared_ptr<int> modified_armor_class = calculate_modified_ac(victim);
		int m_ac = *modified_armor_class;
		auto minimum_crit = attacker->default_attack->minimum_crit_;
		auto second_parameter = attack_value - unmodified_attack_roll;
		auto crit_confirmation_roll = _rng->roll(20);

		//todo: support for other attribute modifiers than just strength.
		auto attribute_modifier = attacker->current_str_modifier;		
		auto atk_request = make_shared<attack_request>(unmodified_attack_roll, second_parameter, minimum_crit, m_ac, crit_confirmation_roll, attribute_modifier);
		auto attack_outcome = _attack_abstraction->get_attack_outcome(atk_request);

		if (attack_outcome != attack_outcome::miss)
		{
			auto returnable = make_shared<attack_results>();
			auto default_attack = attacker->default_attack;
			auto damage_effects = default_attack->damage_effects_;
			auto physical_damage_effect = *std::find_if(damage_effects.begin(), damage_effects.end(), [](damage_effect const& de) { return de._damage_type == damage_type::physical; });
			auto dice = physical_damage_effect.damage_dice[0];
			
			int dice_count = 0;
			int dice_size = 0;

			if (dice != nullptr) {
				dice_count = dice->die_count;
				dice_size = dice->die_size;
			}

			auto crit_multiplier = 2;
			auto the_damage_strategy = make_shared<roll_dice_damage_strategy>(_rng, dice_count, dice_size);


			auto damageRequest = make_shared<damage_request>(attack_outcome, crit_multiplier, the_damage_strategy, attribute_modifier);
			auto physical_damage = _attack_abstraction->get_damage_outcome(damageRequest);

			//todo: update to work with more than just physical damage...
			if (physical_damage > 0)
			{
				auto damage = physical_damage;

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
