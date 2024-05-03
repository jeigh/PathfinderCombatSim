#pragma once

#include <optional>
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <iostream>


using std::vector;
using std::unordered_map;
using std::string;
using std::shared_ptr;
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
		[[nodiscard]] int add_rolls(const vector<shared_ptr<die_roll>> dice) const;
		[[nodiscard]] int roll(const int die_size) const;
	};

	enum damage_type
	{
		none,
		physical = 1,
		poison = 2
	};

	class damage_effect
	{
	public:
		int damage_amount = 0;
		damage_type damage_type = physical;
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
			const int current_hit_points,
			const int max_hit_points,
			const int current_armor_class,
			const int perception_skill_check_modifier,
			shared_ptr<attack> default_attack)
			:
			id(std::move(id)),
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

	using std::cout;
	using std::pair;
	using std::endl;

	class user_interface
	{
	public:
		void die(shared_ptr<mobile_object> target_mob) const
		{
			cout << target_mob->id << " has died!" << endl;
		}

		void knock_out(shared_ptr<mobile_object> target_mob) const
		{
			cout << target_mob->id << " just got knocked out!" << endl;
		}
		void process_attack(shared_ptr<mobile_object> mob, int damage_roll, shared_ptr<mobile_object> victim) const
		{
			// currently uncalled
			cout << "Process attack..." << endl;
		}
		void receive_damage(shared_ptr<attack_results> results) const
		{
			if (results->attacking_mob == nullptr && results->target_mob == nullptr) return;
			cout << results->attacking_mob->id << " has delivered " << results->damage_delivered << " damage to " << results->target_mob->id << endl;
		}
		void round_ends(int turn_id) const
		{
			cout << "Round " << turn_id << " ends." << endl;
		}
		void round_starts(int turn_id) const
		{
			cout << "Round " << turn_id << " starts." << endl;
		}
		void output_aggregates(unordered_map<string, int> const& winners) const
		{
			// currently uncalled
			for (const pair<string, int> winner : winners)
			{
				cout << winner.first << " Wins : " << winner.second << endl;
			}
		}
		void attack_misses(shared_ptr<mobile_object> mob, shared_ptr<mobile_object> target_mob) const
		{
			cout << mob->id << " attempted to hit " << target_mob->id << "  but missed." << endl;
		}
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

		[[nodiscard]] bool contains(shared_ptr<mobile_object> contained) const
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

	class battle_results
	{
	public:
		vector<combat_team> groups;
	};
	class mob_ai
	{
	public:
		explicit mob_ai(shared_ptr<combat_helper> combat) : _combat(combat) { }

	private:
		shared_ptr<combat_helper> _combat;

	public:
		[[nodiscard]] shared_ptr<mobile_object> get_target_for(shared_ptr<mobile_object> item, const vector<shared_ptr<combat_team>>& acgs) const
		{
			for (const shared_ptr<combat_team>& team : acgs)
			{
				if (team->contains(item)) continue;
				for (auto combatant : team->combatants)
				{
					if (!_combat->is_dead(combatant)) 
						return combatant;
				}
			}
			return nullptr;
		}
	};






	class testClassBase
	{
	public:
		virtual void run_test() const = 0;
	};


}
