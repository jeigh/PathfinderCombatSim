#include "types.h"

using std::make_unique;
using std::unique_ptr;
using std::make_shared;
using std::shared_ptr;
using std::tuple;


namespace pathfinder_combat_simulator 
{



	class combat_algorithm : public virtual_combat_algorithm
	{
	public:
		combat_algorithm(const dice_manager& rng, const user_interface& ui, const combat_helper& combat, const mob_ai& ai)
			: _rng(rng), _ui(ui), _combat(combat), _ai(ai) {}

		[[nodiscard]] auto roll_and_order_by_initiative(vector<combat_team> groups) const->vector<shared_ptr<tuple<shared_ptr<mobile_object>, int>>>
		{
			auto initiative_table = vector<shared_ptr<tuple<shared_ptr<mobile_object>, int>>>();

			for (combat_team group : groups)
			{
				for (shared_ptr<mobile_object> combatant : group.combatants)
				{
					int rolled = roll_initiative(combatant);
					auto back_pushable = make_shared<tuple<shared_ptr<mobile_object>, int>>(combatant, rolled);
					initiative_table.push_back(back_pushable);
				}
			}
			return initiative_table;
		}

		[[nodiscard]] auto roll_initiative(shared_ptr<mobile_object> actor) const -> int
		{
			return actor->perception_skill_check_modifier + _rng.roll(20);
		}

		[[nodiscard]] auto is_combat_still_active(const vector<combat_team>& combat_groups) const -> bool
		{
			for (combat_team const& combat_group : combat_groups)
			{
				if (still_lives(combat_group)) return true;
			}
			return false;
		}

		[[nodiscard]] auto roll_damage(vector<damage_effect> damage_effects) const->unique_ptr<unordered_map<damage_type, int>>
		{
			auto returnable = make_unique<unordered_map<damage_type, int>>();

			for (damage_effect damage_effect : damage_effects)
			{
				int amount_to_add = _rng.add_rolls(damage_effect.damage_dice);
				damage_type key = damage_effect.damage_type;

				if (returnable->contains(damage_effect.damage_type))
					returnable->insert_or_assign(key, returnable->at(key) + amount_to_add);
				else
					returnable->insert({ key, amount_to_add });
			}

			return returnable;
		}

		[[nodiscard]] auto roll_to_hit(int number_of_previous_attacks_this_turn, int attack_modifier) const -> int
		{
			return _rng.roll(20) + attack_modifier - (number_of_previous_attacks_this_turn * 5);
		}

		[[nodiscard]] auto process_battle_until_only_one_team_is_concious(vector<combat_team> combat_groups) const->battle_results;

	private:
		dice_manager _rng;
		user_interface _ui;
		combat_helper _combat;
		mob_ai _ai;

		[[nodiscard]] auto still_lives(const combat_team& acg) const -> bool
		{
			for (auto mob : acg.combatants)
			{
				if (!_combat.is_dead(mob)) return true;
			}
			return false;
		}

		[[nodiscard]] auto attack(shared_ptr<mobile_object> attacker, shared_ptr<mobile_object> victim, int number_of_previous_attacks_made_this_turn) const->shared_ptr<attack_results>
		{
			if (attacker->default_attack == nullptr) return nullptr;

			const int attack_value = roll_to_hit(number_of_previous_attacks_made_this_turn, attacker->default_attack->attack_modifier_);
			if (attack_value > victim->current_armor_class)
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

		void process_action(int action_number, shared_ptr<mobile_object> acting_mob, vector <combat_team> combat_groups, unordered_map<shared_ptr<mobile_object>, int> dictionary_of_previous_attack_counts) const
		{
			auto target_mob = _ai.get_target_for(acting_mob, combat_groups);
			if (target_mob == nullptr || _combat.is_dead(acting_mob) || _combat.is_unconcious(acting_mob))
			{
				auto victim = target_mob;
				auto prev_attacks = dictionary_of_previous_attack_counts.at(acting_mob);
				auto results = attack(acting_mob, victim, prev_attacks);
				if (results != nullptr && !(results->target_mob == nullptr))
					_ui.attack_misses(acting_mob, victim);
				else
				{
					if (results->damage_delivered <= 0) return;
					results->target_mob->current_hit_points -= results->damage_delivered;
					_ui.receive_damage(results);
				}
			}
		}

		[[nodiscard]] auto has_available_action_this_turn(mobile_object mob) const -> bool
		{
			return true;
		}

	};






























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
