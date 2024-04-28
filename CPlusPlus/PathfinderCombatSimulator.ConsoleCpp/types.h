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
		[[nodiscard]] int add_rolls(const vector<die_roll> dice) const;
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
			const attack& default_attack)
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

	class user_interface
	{
	public:
		void die(mobile_object target_mob) const;
		void flush() const;
		void knock_out(mobile_object target_mob) const;
		void process_attack(mobile_object mob, int damage_roll, mobile_object victim) const;
		void receive_damage(attack_results results) const;
		void round_ends(int turn_id) const;
		void round_starts(int turn_id) const;
		void output_aggregates(unordered_map<string, int> const& winners) const;
		void attack_misses(mobile_object mob, mobile_object target_mob) const;
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
		vector<mobile_object> combatants;
	};

	class battle_results {};
	class mob_ai {};

	class combat_helper {
	public:
		[[nodiscard]] bool is_dead(const mobile_object& mob) const;
		[[nodiscard]] bool is_unconcious(const mobile_object& mob) const;
	};

	class combat_algorithm
	{
	public:
		combat_algorithm(const dice_manager& rng, const user_interface& ui, const combat_helper& combat, const mob_ai& ai)
			: _rng(rng), _ui(ui), _combat(combat), _ai(ai) {}

		[[nodiscard]] auto roll_and_order_by_initiative(vector<combat_team> groups) const -> unordered_map<mobile_object, int>;
		[[nodiscard]] auto roll_initiative(mobile_object actor) const -> int;
		[[nodiscard]] auto is_combat_still_active(const vector<combat_team>& combat_groups) const -> bool;
		[[nodiscard]] auto roll_damage(vector<damage_effect> damage_effects) const -> unordered_map<string, int>;
		[[nodiscard]] auto roll_to_hit(int number_of_previous_attacks_this_turn, int attack_modifier) const -> int;
		[[nodiscard]] auto process_battle_until_only_one_team_is_concious(vector<combat_team> combat_groups) const -> battle_results;

	private:
		dice_manager _rng;
		user_interface _ui;
		combat_helper _combat;
		mob_ai _ai;

		[[nodiscard]] auto still_lives(const combat_team& acg) const -> bool;
		[[nodiscard]] auto attack(mobile_object target, mobile_object victim, int number_of_previous_attacks_made_this_turn) const -> attack_results;
		void process_action(int action_number, mobile_object mob, vector <combat_team> combatGroups, unordered_map<mobile_object, int> dictionary_of_previous_attack_counts) const;
		[[nodiscard]] auto has_available_action_this_turn(mobile_object mob) const -> bool;

	};


}
