#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <fstream>

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

	class data_collection_payload {
	private:
		shared_ptr<mobile_object> attacker = nullptr;
		shared_ptr<mobile_object> defender = nullptr;

	public:
		//todo: some of these need to be populated 
		int unmodified_attack_roll = 0;
		int attackers_momentary_attack_bonus = 0;
		int defenders_momentary_ac = 0;
		int attakcer_number_of_attacks_made_this_turn = 0;
		bool attack_successfully_hits = false;
		bool attacker_wins_battle = false;
		int attackers_weapons_minimum_crit = 0;
		int attackers_weapons_crit_multipler = 0;
		float predicted_damage_if_hit = 0;


		void set_attacker(shared_ptr<mobile_object> attacker) { this->attacker = attacker; }
		void set_defender(shared_ptr<mobile_object> defender) { this->defender = defender; }
	};

	class battle
	{
	public:
		battle(int id, const vector<std::shared_ptr<combat_team>>& combat_teams) :
			id(id), combat_teams(combat_teams) { }

		int id;
		vector<shared_ptr<combat_team>> combat_teams;
		vector<shared_ptr<data_collection_payload>> collected_data = vector<shared_ptr<data_collection_payload>>();
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

	enum class attack_outcome
	{
		hit_no_crit = 1,
		miss = 2,
		hit_and_crit = 3
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

	class attack_scenario
	{
	public:
		
		int attack_bonus;
		int armor_class;
		int minimum_crit;
		int crit_multiplier;
		int unmodified_attack_roll;
		int damage_dice_count;
		float mean_damage_per_die;
		float expected_result;
	};

	class damage_strategy {
	public:
		damage_strategy(int damage_dice_count, int damage_dice_size) : _damage_dice_count(damage_dice_count), _damage_dice_size(damage_dice_size) { }

		virtual float get_damage_as_float() const = 0;
		int get_damage_as_int() const;


		int _damage_dice_count;
		int _damage_dice_size;
	};

	class damage_request {
	public:
		damage_request(attack_outcome this_attack_outcome, int crit_multiplier, shared_ptr<damage_strategy> dmg_strategy) :
			this_attack_outcome(this_attack_outcome), crit_multiplier(crit_multiplier), dmg_strategy(dmg_strategy) { }

		attack_outcome this_attack_outcome;
		int crit_multiplier;
		
		shared_ptr<damage_strategy> dmg_strategy;
	};

	class attack_request {
	public:
		attack_request(int unmodified_attack_roll, int attack_bonus, int minimum_crit, int armor_class) : 
			unmodified_attack_roll(unmodified_attack_roll), attack_bonus(attack_bonus), minimum_crit(minimum_crit), armor_class(armor_class) { }

		int unmodified_attack_roll;
		int attack_bonus;
		int minimum_crit;
		int armor_class;
	};

	class data_access
	{

	public:
		explicit data_access(shared_ptr<std::shared_mutex> db_mutex) : _db_mutex(db_mutex) {}
		~data_access()
		{
			if (_attackfile.is_open()) _attackfile.close();
			if (_damagefile.is_open()) _damagefile.close();
		}

		void persist_attack_results(shared_ptr<attack_request> request, attack_outcome outcome);
		void persist_damage_results(int damage_dice_count, float statistical_damage_mean, shared_ptr<damage_request> request, float expected_result);

	private:
		std::string _attack_scenario_file_name = "attack_scenarios.csv";
		std::string _damage_scenario_file_name = "damage_scenarios.csv";

		std::ofstream _attackfile;
		std::ofstream _damagefile;

		shared_ptr<std::shared_mutex> _db_mutex;
	};

	class attack_abstraction
	{
	public:
		attack_abstraction(shared_ptr<dice_manager> rng, shared_ptr<data_access> dal) : _rng(rng), _dal(dal) { }

		attack_outcome get_attack_outcome(shared_ptr<attack_request> request);
		float get_damage_outcome(shared_ptr<damage_request> request);
	private:
		attack_outcome get_attack_outcome(int unmodified_attack_roll, int attackers_attack_bonus, int attackers_weapons_minimum_crit, int defenders_ac);
		float get_damage_outcome(attack_outcome the_attack_outcome, int attackers_weapons_crit_multiplier,  shared_ptr<damage_strategy> dmg_strategy);

		shared_ptr<dice_manager> _rng;
		shared_ptr<data_access> _dal;
	};

	class attack_process
	{
	public:
		attack_process(shared_ptr<data_access> dal, shared_ptr<dice_manager> rng) : _dal(dal), _rng(rng) { }

		[[nodiscard]] shared_ptr<attack_results> do_attack(shared_ptr<battle> the_battle, shared_ptr<mobile_object> const attacker, shared_ptr<mobile_object> const victim);
		[[nodiscard]] int apply_attack_modifiers(shared_ptr<mobile_object> mob, int d20) const;
		[[nodiscard]] shared_ptr<unordered_map<damage_type, int>> roll_damage(vector<damage_effect> const damage_effects) const;

	private:
		shared_ptr<dice_manager> _rng;
		shared_ptr<data_access> _dal;

		[[nodiscard]] shared_ptr<int> calculate_modified_ac(shared_ptr<mobile_object> const victim) const;
		void collect_data_payload(const std::shared_ptr<pathfinder_combat_simulator::mobile_object>& attacker, const std::shared_ptr<pathfinder_combat_simulator::mobile_object>& victim, const int unmodified_attack_roll);
	};

	class combat_process
	{
	public:
		combat_process(shared_ptr<dice_manager> rng, shared_ptr<user_interface> ui, shared_ptr<mob_ai> ai, shared_ptr<data_access> dal) :
			_rng(rng), _ui(ui), _ai(ai)
		{
			_attack_process = std::make_shared<attack_process>(dal, rng);
		}

		[[nodiscard]] shared_ptr<initiative_table> roll_and_order_by_initiative(shared_ptr<battle> the_battle) const;
		[[nodiscard]] int roll_initiative(shared_ptr<mobile_object> const actor) const;
		[[nodiscard]] bool is_combat_still_active(vector<shared_ptr<combat_team>> const combat_groups) const;

		void process_battle_until_only_one_team_is_concious(const shared_ptr<battle>);

	private:
		shared_ptr<dice_manager> _rng;
		shared_ptr<user_interface> _ui;
		shared_ptr<mob_ai> _ai;
		shared_ptr<attack_process> _attack_process;

		[[nodiscard]] bool still_lives(shared_ptr<combat_team> const acg) const;
		[[nodiscard]] bool has_available_action_this_turn(shared_ptr<mobile_object> const mob) const;

		void process_action(shared_ptr<battle> the_battle, int const action_number, shared_ptr<mobile_object> acting_mob);
	};

	class battle_results
	{
	public:
		vector<combat_team> groups;
	};

	class statistical_mean_damage_strategy : public damage_strategy {
	public:
		statistical_mean_damage_strategy(int damage_dice_count, int damage_dice_size) : damage_strategy(damage_dice_count, damage_dice_size) { }

		float get_damage_as_float() const;
	};

	class roll_dice_damage_strategy : public damage_strategy
	{
	public:
		roll_dice_damage_strategy(int damage_dice_count, int damage_dice_size) : damage_strategy(damage_dice_count, damage_dice_size) { }

		float get_damage_as_float() const;
	};
}
