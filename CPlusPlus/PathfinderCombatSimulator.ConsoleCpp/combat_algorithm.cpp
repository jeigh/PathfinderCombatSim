#include "types.h"


namespace pathfinder_combat_simulator 
{
	[[nodiscard]] auto combat_algorithm::roll_initiative(mobile_object actor) const -> int
	{
		return actor.perception_skill_check_modifier + _rng.roll(20);
	}

	[[nodiscard]] auto combat_algorithm::has_available_action_this_turn(mobile_object mob) const -> bool
	{
		return true;
	}

	[[nodiscard]] auto combat_algorithm::roll_to_hit(int number_of_previous_attacks_this_turn, int attack_modifier) const -> int
	{
		return _rng.roll(20) + attack_modifier - (number_of_previous_attacks_this_turn * 5);
	}

	[[nodiscard]] auto combat_algorithm::still_lives(const combat_team& acg) const -> bool
	{
		for (const mobile_object& mob : acg.combatants)
		{
			if (!_combat.is_dead(mob)) return true;
		}
		return false;
	}

	[[nodiscard]] auto combat_algorithm::is_combat_still_active(const vector<combat_team>& combat_groups) const -> bool
	{
		for (combat_team const& combat_group : combat_groups)
		{
			if (still_lives(combat_group)) return true;
		}
		return false;
	}



	//[[nodiscard]] auto combat_algorithm::roll_damage(vector<damage_effect> damage_effects) const->unordered_map<string, int>
	//{
	//	
	//}

	//[[nodiscard]] auto combat_algorithm::process_battle_until_only_one_team_is_concious(vector<combat_team> combat_groups) const->battle_results
	//{
	//	//if (combat_groups.size == 2)
	//	//{
	//	//	unordered_map<mobile_object, int> sorted_initiative = roll_and_order_by_initiative(combat_groups);

	//	//	int roundId = 1;
	//	//	while (is_combat_still_active(combat_groups))
	//	//	{
	//	//		_ui.round_starts(roundId);
	//	//		unordered_map<mobile_object, int> previous_attack_count_for_mob_this_round =  unordered_map<mobile_object, int>();
	//	//		//for (unordered_map<mobile_object, int> item : sorted_initiative)
	//	//		//{
	//	//			//if (has_available_action_this_turn(item)) process_action(1, item.mob, combat_groups, previous_attack_count_for_mob_this_round);
	//	//			//if (has_available_action_this_turn(item.mob)) process_action(2, item.mob, combat_groups, previous_attack_count_for_mob_this_round);
	//	//			//if (has_available_action_this_turn(item.mob)) process_action(3, item.mob, combat_groups, previous_attack_count_for_mob_this_round);
	//	//		//}
	//	//		_ui.round_ends(roundId);

	//	//		roundId++;
	//	//	}

	//	//	return new battle_results() { groups = combat_groups };

	//	//}

	//}

	//[[nodiscard]] auto combat_algorithm::attack(mobile_object target, mobile_object victim, int number_of_previous_attacks_made_this_turn) const->attack_results
	//{
	//	
	//}

	//void combat_algorithm::process_action(int action_number, mobile_object mob, vector <combat_team> combatGroups, unordered_map<mobile_object, int> dictionary_of_previous_attack_counts) const
	//{
	//	
	//}

	//[[nodiscard]] auto combat_algorithm::roll_and_order_by_initiative(vector<combat_team> groups) const->unordered_map<mobile_object, int>
	//{
		//vector<mobile_object, int> initiative_table;

		//for(auto group : groups)
		//{
		//	for(mobile_object mob : group.combatants)
		//	{
		//		initiative_table.push_back(mob, roll_initiative(mob));
		//	}
		//}

		//return (from item in initiativeTable orderby item.initiativeRoll select item).ToList();

	//}
}
