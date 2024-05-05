#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include <shared_mutex>


using std::vector;
using std::unordered_map;
using std::string;
using std::unique_ptr;
using std::tuple;

#define nullable std::shared_ptr

namespace pathfinder_combat_simulator 
{

	enum damage_type
	{
		none,
		physical = 1,
		poison = 2
	};

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

	class damage_effect
	{
	public:
		int damage_amount = 0;
		damage_type _damage_type = physical;
		vector<nullable<die_roll>> damage_dice;
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
			const int unbuffed_max_hit_points,
			const int unmodified_current_armor_class,
			const int perception_skill_check_modifier,
			nullable<attack> default_attack)
			:
			id(std::move(id)),
			current_hit_points(unbuffed_max_hit_points),
			unbuffed_max_hit_points(unbuffed_max_hit_points),
			unmodified_current_armor_class(unmodified_current_armor_class),
			perception_skill_check_modifier(perception_skill_check_modifier),
			default_attack(default_attack) { }

		string id;
		int current_hit_points = 0;
		int unbuffed_max_hit_points = 0;
		int unmodified_current_armor_class = 0;
		int perception_skill_check_modifier = 0;
		nullable<attack> default_attack = nullptr;

		[[nodiscard]] bool is_dead() const;
		[[nodiscard]] bool is_unconcious() const;
		[[nodiscard]] bool is_prone() const;


	private:
		bool prone = false;
		[[nodiscard]] bool operator==(const mobile_object& contained) const;
		[[nodiscard]] bool operator==(const nullable<mobile_object>& shared) const;
	};


	class combat_team
	{
	public:
		string name;
		vector<nullable<mobile_object>> combatants;

		[[nodiscard]] auto contains(nullable<mobile_object> contained) const -> bool;

	};

	class battle
	{
	public:
		battle(int id, const vector<std::shared_ptr<combat_team>>& combat_teams) :
			id(id), combat_teams(combat_teams) { }

		int id;
		vector<nullable<combat_team>> combat_teams;
	};

	class attack_results
	{
	public:
		attack_results() = default;

		int damage_delivered = 0;
		nullable<mobile_object> target_mob;
		nullable<mobile_object> attacking_mob;
	};

	enum class output_level
	{
		none = 0,
		low = 1,
		medium = 2,
		high = 3,
		max_verbosity = 4
	};


	class user_interface
	{
	public:
		explicit user_interface(nullable<std::shared_mutex> ui_mutex, output_level level) : _ui_mutex(ui_mutex), _level(level) {	}

		void die(nullable<battle>, nullable<mobile_object> target_mob) const;
		void knock_out(nullable<battle>, nullable<mobile_object> target_mob) const;
		void process_attack(nullable<battle>, nullable<mobile_object> mob, int damage_roll, nullable<mobile_object> victim) const;
		void receive_damage(nullable<battle>, nullable<attack_results> results) const;
		void round_ends(nullable<battle>, int turn_id) const;
		void round_starts(nullable<battle>, int turn_id) const;
		void output_aggregates(nullable<battle>, unordered_map<string, int> const& winners) const;
		void attack_misses(nullable<battle>, nullable<mobile_object> mob, const nullable<mobile_object> target_mob) const;
	private:
		nullable<std::shared_mutex> _ui_mutex;
		output_level _level;
	};

	class mob_and_initiative
	{
	public:
		mobile_object mob;
		int initiative_roll;
	};




	class mob_ai
	{
	public:
		explicit mob_ai() = default;

		[[nodiscard]] auto get_target_for(nullable<mobile_object> item, const vector<nullable<combat_team>>& acgs) const ->
			nullable<mobile_object>;

	};

	class dice_manager
	{
	public:
		[[nodiscard]] int add_rolls(const vector<nullable<die_roll>> dice) const;
		[[nodiscard]] int roll(const int die_size) const;
	};


	class combat_algorithm
	{
	public:
		combat_algorithm(
			nullable<dice_manager> rng,
			nullable<user_interface> ui,
			nullable<mob_ai> ai)
			:
			_rng(rng),
			_ui(ui),
			_ai(ai) {}

		[[nodiscard]] vector<nullable<tuple<nullable<mobile_object>, int>>> roll_and_order_by_initiative(vector<nullable<combat_team>> const groups) const;
		[[nodiscard]] int roll_initiative(nullable<mobile_object> const actor) const;
		[[nodiscard]] bool is_combat_still_active(vector<nullable<combat_team>> const combat_groups) const;
		[[nodiscard]] nullable<unordered_map<damage_type, int>> roll_damage(vector<damage_effect> const damage_effects) const;
		[[nodiscard]] int roll_to_hit(int const number_of_previous_attacks_this_turn, int const attack_modifier) const;

		void process_battle_until_only_one_team_is_concious(const nullable<battle>) const;


	private:
		nullable<dice_manager> _rng;
		nullable<user_interface> _ui;
		nullable<mob_ai> _ai;

		[[nodiscard]] bool still_lives(nullable<combat_team> const acg) const;
		[[nodiscard]] nullable<int> calculate_modified_ac(nullable<mobile_object> const victim) const;
		[[nodiscard]] nullable<attack_results> attack(nullable<mobile_object> const attacker, nullable<mobile_object> const victim, int const number_of_previous_attacks_made_this_turn) const;
		[[nodiscard]] bool has_available_action_this_turn(nullable<mobile_object> const mob) const;

		void process_action(nullable<battle> the_battle, int const action_number, nullable<mobile_object> acting_mob, unordered_map<nullable<mobile_object>, int> dictionary_of_previous_attack_counts) const;
		

	};

	class battle_results
	{
	public:
		vector<combat_team> groups;
	};




}
