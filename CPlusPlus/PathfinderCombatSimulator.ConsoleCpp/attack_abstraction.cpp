#include <algorithm>
#include "types.h"

using namespace pathfinder_combat_simulator;

attack_outcome get_attack_outcome(int unmodified_attack_roll, int attackers_attack_bonus, int attackers_weapons_minimum_crit, int defenders_ac)
{
	bool is_nat_1 = unmodified_attack_roll == 1;
	bool is_nat_20 = unmodified_attack_roll == 20;
	auto modified_attack_roll = unmodified_attack_roll + attackers_attack_bonus;
	auto is_a_crit_if_it_hits = modified_attack_roll >= attackers_weapons_minimum_crit;
	bool is_a_hit = is_nat_20 || modified_attack_roll >= defenders_ac;

	if (is_nat_1) return attack_outcome::miss;
	if (is_a_hit && is_a_crit_if_it_hits) return attack_outcome::hit_and_crit;
	if (is_a_hit && !is_a_crit_if_it_hits) return attack_outcome::hit_no_crit;

	else return attack_outcome::miss;
}


float attack_abstraction::simulate_attack(
	int attackers_attack_bonus,
	int defenders_ac,
	int attackers_weapons_minimum_crit,
	int attackers_weapons_crit_multipler,
	int unmodified_attack_roll,
	shared_ptr<damage_strategy> dmg_strategy) const {

	auto this_attack_outcome = get_attack_outcome(unmodified_attack_roll, attackers_attack_bonus, attackers_weapons_minimum_crit, defenders_ac);
	float returnable = 0.0f;

	switch (this_attack_outcome) {
		case attack_outcome::hit_and_crit:
			returnable = dmg_strategy->get_damage_as_float() * attackers_weapons_crit_multipler;
			break;
		case attack_outcome::hit_no_crit:
			returnable = dmg_strategy->get_damage_as_float();
			break;
		default:
			returnable = 0.0f;
			break;
	}

	return returnable;
}

float statistical_mean_damage_strategy::get_damage_as_float() const 
{
	auto returnable = 0.5 * _damage_dice_size + 0.5;
	return returnable;
}

float roll_dice_damage_strategy::get_damage_as_float() const {
	//todo: implement dice roll
	return 0.0f;
}

int damage_strategy::get_damage_as_int() const {
	int returnable = static_cast<int>(std::round(get_damage_as_float()));
	return returnable;
}
	


