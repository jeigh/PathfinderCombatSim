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
using std::shared_ptr;

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
		vector<shared_ptr<die_roll>> damage_dice;
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
			shared_ptr<attack> default_attack)
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
		int number_of_previous_attacks_made_this_turn = 0;
		shared_ptr<attack> default_attack = nullptr;

		[[nodiscard]] bool is_dead() const;
		[[nodiscard]] bool is_unconcious() const;
		[[nodiscard]] bool is_prone() const;


	private:
		bool prone = false;
		[[nodiscard]] bool operator==(const mobile_object& contained) const;
		[[nodiscard]] bool operator==(const shared_ptr<mobile_object>& shared) const;
	};


	class combat_team
	{
	public:
		string name;
		vector<shared_ptr<mobile_object>> combatants;

		[[nodiscard]] auto contains(shared_ptr<mobile_object> contained) const -> bool;

	};

	class battle
	{
	public:
		battle(int id, const vector<std::shared_ptr<combat_team>>& combat_teams) :
			id(id), combat_teams(combat_teams) { }

		int id;
		vector<shared_ptr<combat_team>> combat_teams;
		void round_ends();

	private:
		[[nodiscard]] vector<shared_ptr<mobile_object>> get_combatants() const;
	};

	class attack_results
	{
	public:
		attack_results() = default;

		int damage_delivered = 0;
		shared_ptr<mobile_object> target_mob;
		shared_ptr<mobile_object> attacking_mob;
	};

	enum class output_level
	{
		none = 0,
		low = 1,
		mediumlow = 2,
		medium = 3,
		high = 4,
		max = 5
	};


	class user_interface
	{
	public:
		explicit user_interface(shared_ptr<std::shared_mutex> ui_mutex, output_level level) : _ui_mutex(ui_mutex), _level(level) {	}

		void die(shared_ptr<battle>, shared_ptr<mobile_object> target_mob) const;
		void knock_out(shared_ptr<battle>, shared_ptr<mobile_object> target_mob) const;
		void process_attack(shared_ptr<battle>, shared_ptr<mobile_object> mob, int damage_roll, shared_ptr<mobile_object> victim) const;
		void receive_damage(shared_ptr<battle>, shared_ptr<attack_results> results) const;
		void round_ends(shared_ptr<battle>, int turn_id) const;
		void round_starts(shared_ptr<battle>, int turn_id) const;
		void output_aggregates(shared_ptr<battle>, unordered_map<string, int> const& winners) const;
		void attack_misses(shared_ptr<battle>, shared_ptr<mobile_object> mob, const shared_ptr<mobile_object> target_mob) const;
	private:
		shared_ptr<std::shared_mutex> _ui_mutex;
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

		[[nodiscard]] auto get_target_for(shared_ptr<mobile_object> item, const vector<shared_ptr<combat_team>>& acgs) const ->
			shared_ptr<mobile_object>;

	};

	class dice_manager
	{
	public:
		[[nodiscard]] int add_rolls(const vector<shared_ptr<die_roll>> dice) const;
		[[nodiscard]] int roll(const int die_size) const;
	};

	class initiative_entry
	{
	public:
		shared_ptr<mobile_object> mob;
		int initiative_roll = 0;
	};


	class initiative_table
	{
	public:
		void add_roll_by_mob(shared_ptr<mobile_object>, int);
		vector<shared_ptr<mobile_object>> get_ordered_mobs();

	private:
		vector<shared_ptr<initiative_entry>> raw;
	};

	class combat_process
	{
	public:
		combat_process(shared_ptr<dice_manager> rng, shared_ptr<user_interface> ui, shared_ptr<mob_ai> ai) :
			_rng(rng), _ui(ui), _ai(ai) {}

		
		[[nodiscard]] shared_ptr<initiative_table> roll_and_order_by_initiative(shared_ptr<battle> the_battle) const;
		[[nodiscard]] int roll_initiative(shared_ptr<mobile_object> const actor) const;
		[[nodiscard]] bool is_combat_still_active(vector<shared_ptr<combat_team>> const combat_groups) const;
		[[nodiscard]] shared_ptr<unordered_map<damage_type, int>> roll_damage(vector<damage_effect> const damage_effects) const;
		//[[nodiscard]] int roll_to_hit(nullable<mobile_object> mob, int const attack_modifier) const;
		[[nodiscard]] int roll_to_hit(shared_ptr<mobile_object> mob) const;

		void process_battle_until_only_one_team_is_concious(const shared_ptr<battle>);

	private:
		shared_ptr<dice_manager> _rng;
		shared_ptr<user_interface> _ui;
		shared_ptr<mob_ai> _ai;

		[[nodiscard]] bool still_lives(shared_ptr<combat_team> const acg) const;
		[[nodiscard]] shared_ptr<int> calculate_modified_ac(shared_ptr<mobile_object> const victim) const;
		[[nodiscard]] shared_ptr<attack_results> attack(shared_ptr<mobile_object> const attacker, shared_ptr<mobile_object> const victim) const;
		[[nodiscard]] bool has_available_action_this_turn(shared_ptr<mobile_object> const mob) const;

		void process_action(shared_ptr<battle> the_battle, int const action_number, shared_ptr<mobile_object> acting_mob) const;
	};

	class battle_results
	{
	public:
		vector<combat_team> groups;
	};




}
