#pragma once

#include <unordered_map>
#include <vector>
#include <string>


using std::vector;

using std::unordered_map;
using std::string;


namespace pathfinder_combat_simulator 
{

	class die_roll
	{
	public:
		die_roll(const int die_count, const int die_size, const int modifier_after_addition) : die_size(die_size), die_count(die_count), modifer_after_addition(modifier_after_addition) {}
		die_roll(const int die_count, const int die_size) : die_size(die_size), die_count(die_count) {}
		die_roll(const int die_size) : die_size(die_size) {}
		die_roll() = default;

		int die_size = 0;
		int die_count = 1;
		int modifer_after_addition = 0;
	};

	class dice_manager
	{
	public:
		[[nodiscard]] int add_rolls(const vector<die_roll>& dice) const;

	private:
		[[nodiscard]] int roll(const int die_size) const;
	};

	class damage_effect
	{
	public:
		int damage_amount;
		string damage_type;
	};

	class attack
	{
	public:
		vector <damage_effect> damage_effects_;
		int attack_modifier_ = 0;
	};


	class mobile_object
	{
	public:
		mobile_object(string id,
			const int current_hit_points,
			const int max_hit_points,
			const int current_armor_class,
			const int perception_skill_check_modifier,
			attack default_attack)
			: id(std::move(id)),
			current_hit_points(current_hit_points),
			max_hit_points(max_hit_points),
			current_armor_class(current_armor_class),
			perception_skill_check_modifier(perception_skill_check_modifier),
			default_attack(std::move(default_attack)) { }

		string id;
		int current_hit_points = 0;
		int max_hit_points = 0;
		int current_armor_class = 0;
		int perception_skill_check_modifier = 0;
		attack default_attack;
	};

	class attack_results
	{
	public:
		int damage_delivered = 0;
		mobile_object target_mob;
		mobile_object attacking_mob;
	};

	class mob_and_initiative
	{
	public:
		mobile_object mob;
		int initiative_roll;
	};

	class combat_team
	{
	public:
		string name;

	};
	class battle_results {};
	class user_interface {};
	class combat_helper {};
	class mob_ai {};

	class combat_algorithm
	{
	public:
		combat_algorithm(const dice_manager& rng, const user_interface& ui, const combat_helper& combat, const mob_ai& ai)
			: _rng(rng), _ui(ui), _combat(combat), _ai(ai) {}

		[[nodiscard]] unordered_map<mobile_object, int> roll_and_order_by_initiative(vector <combat_team> groups) const;
		[[nodiscard]] int roll_initiative(mobile_object actor) const;
		[[nodiscard]] bool is_combat_still_active(vector <combat_team> combat_groups) const;
		[[nodiscard]] unordered_map <string, int> roll_damage(vector <damage_effect> damage_effects) const;
		[[nodiscard]] int roll_to_hit(int number_of_previous_attacks_this_turn, int attack_modifier) const;
		[[nodiscard]] battle_results process_battle_until_only_one_team_is_concious(vector <combat_team> combat_groups) const;

	private:
		dice_manager _rng;
		user_interface _ui;
		combat_helper _combat;
		mob_ai _ai;

		[[nodiscard]] bool still_lives(combat_team acg) const;
		[[nodiscard]] attack_results  attack(mobile_object target, mobile_object victim, int number_of_previous_attacks_made_this_turn) const;
		void process_action(int action_number, mobile_object mob, vector <combat_team> combatGroups, unordered_map<mobile_object, int> dictionary_of_previous_attack_counts) const;
		[[nodiscard]] bool has_available_action_this_turn(mobile_object mob) const;

	};


}
