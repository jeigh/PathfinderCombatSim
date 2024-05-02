#pragma once

#include <optional>
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <tuple>

using std::vector;
using std::unordered_map;
using std::string;
using std::shared_ptr;
using std::optional;
using std::nullopt;
using std::unique_ptr;
using std::tuple;

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
		[[nodiscard]] int roll(const int die_size) const;
	};

	enum damage_type
	{
		none,
		physical,
		poison
	};

	class damage_effect
	{
	public:
		int damage_amount;
		damage_type damage_type;
		vector<die_roll> damage_dice;
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
			shared_ptr<attack> default_attack)
			: id(std::move(id)),
			current_hit_points(current_hit_points),
			max_hit_points(max_hit_points),
			current_armor_class(current_armor_class),
			perception_skill_check_modifier(perception_skill_check_modifier),
			default_attack(default_attack) { }

		string id;
		int current_hit_points = 0;
		int max_hit_points = 0;
		int current_armor_class = 0;
		int perception_skill_check_modifier = 0;
		shared_ptr<attack> default_attack = nullptr;

		bool operator==(const mobile_object& contained) const
		{
			return this->id == contained.id;
		}

		bool operator==(const shared_ptr<mobile_object>& shared) const
		{
			return this->id == shared->id;
		}
	};




	class attack_results
	{
	public:
		attack_results() = default;

		int damage_delivered = 0;
		shared_ptr<mobile_object> target_mob;
		shared_ptr<mobile_object> attacking_mob;
	};

	class user_interface
	{
	public:
		void die(mobile_object target_mob) const;
		void flush() const;
		void knock_out(mobile_object target_mob) const;
		void process_attack(mobile_object mob, int damage_roll, mobile_object victim) const;
		void receive_damage(shared_ptr<attack_results> results) const;
		void round_ends(int turn_id) const;
		void round_starts(int turn_id) const;
		void output_aggregates(unordered_map<string, int> const& winners) const;
		void attack_misses(shared_ptr<mobile_object> mob, shared_ptr<mobile_object> target_mob) const;
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
		vector<shared_ptr<mobile_object>> combatants;
		bool contains(shared_ptr<mobile_object> contained) const
		{
			for (auto mob : combatants)
			{
				if (mob == contained) return true;
			}
			return false;
		}
	};

	class combat_helper {
	public:
		[[nodiscard]] bool is_dead(shared_ptr<mobile_object> mob) const;
		[[nodiscard]] bool is_unconcious(shared_ptr<mobile_object> mob) const;
	};

	class battle_results {};
	class mob_ai
	{
	public:
		explicit mob_ai(const combat_helper& combat) : _combat(combat) { }

	private:
		combat_helper _combat;

	public:
		[[nodiscard]] shared_ptr<mobile_object> get_target_for(shared_ptr<mobile_object> item, const vector<combat_team>& acgs) const
		{
			for (const combat_team& team : acgs)
			{
				if (team.contains(item)) continue;
				for (auto combatant : team.combatants)
				{
					if (!_combat.is_dead(combatant)) 
						return combatant;
				}
			}
			return nullptr;
		}
	};

	class virtual_combat_algorithm
	{
	public:
		
		[[nodiscard]] virtual auto roll_and_order_by_initiative(vector<combat_team> groups) const->vector<shared_ptr<tuple<shared_ptr<mobile_object>, int>>> = 0;
		[[nodiscard]] virtual auto roll_initiative(mobile_object actor) const -> int = 0;
		[[nodiscard]] virtual auto is_combat_still_active(const vector<combat_team>& combat_groups) const -> bool = 0;
		[[nodiscard]] virtual auto roll_damage(vector<damage_effect> damage_effects) const->unique_ptr<unordered_map<damage_type, int>> = 0;
		[[nodiscard]] virtual auto roll_to_hit(int number_of_previous_attacks_this_turn, int attack_modifier) const -> int = 0;
		[[nodiscard]] virtual auto process_battle_until_only_one_team_is_concious(vector<combat_team> combat_groups) const->battle_results = 0;
	};




	class testClassBase
	{
	public:
		virtual void run_test() const = 0;
	};


}
