#include <algorithm>
#include "types.h"

using namespace pathfinder_combat_simulator;

attack_outcome attack_abstraction::get_attack_outcome(shared_ptr<attack_request> request)
{
	bool attack_is_nat_1 = request->unmodified_attack_roll == 1;
	bool attack_is_nat_20 = request->unmodified_attack_roll == 20;

	bool crit_is_nat_1 = request->crit_confirmation_roll == 1;
	bool crit_is_nat_20 = request->crit_confirmation_roll == 20;

	int modified_attack_roll = request->unmodified_attack_roll + request->attack_bonus;
	int modified_crit_roll = request->crit_confirmation_roll + request->attack_bonus;
	
	bool is_a_crit_if_it_hits = attack_is_nat_20 && (crit_is_nat_20 || modified_crit_roll >= request->minimum_crit);
	bool is_a_hit = attack_is_nat_20 || modified_attack_roll >= request->armor_class;

	if (attack_is_nat_1) return attack_outcome::miss;
	if (is_a_hit && is_a_crit_if_it_hits) return attack_outcome::hit_and_crit;
	if (is_a_hit && !is_a_crit_if_it_hits) return attack_outcome::hit_no_crit;

	else return attack_outcome::miss;
}

float attack_abstraction::get_damage_outcome(shared_ptr<damage_request> request)
{
	float returnable = 0.0f;

	switch (request->this_attack_outcome) {
	case attack_outcome::hit_and_crit:
		returnable = request->dmg_strategy->get_damage_as_float() * request->crit_multiplier + request->attribute_modifier;
		break;
	case attack_outcome::hit_no_crit:
		returnable = request->dmg_strategy->get_damage_as_float() + request->attribute_modifier;
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
	float returnable = 0.0f;

	for (int i = 0; i < _damage_dice_count; i++)
	{
		returnable += _rng->roll(_damage_dice_size);
	}
	return returnable;
}

int damage_strategy::get_damage_as_int() const {
	int returnable = static_cast<int>(std::round(get_damage_as_float()));
	return returnable;
}
	


