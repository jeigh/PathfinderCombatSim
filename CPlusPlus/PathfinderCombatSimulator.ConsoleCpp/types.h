#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <iostream>


using std::vector;
using std::unordered_map;
using std::string;
using std::unique_ptr;
using std::tuple;

#define nullable std::shared_ptr

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
		[[nodiscard]] int add_rolls(const vector<nullable<die_roll>> dice) const;
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

		[[nodiscard]] bool is_dead() const
		{
			return current_hit_points <= -10;
		}

		[[nodiscard]] bool is_unconcious() const
		{
			return current_hit_points > -10 && current_hit_points <= 0;
		}

		[[nodiscard]] bool is_prone() const
		{
			return prone;
		}


	private:
		bool prone = false;


		bool operator==(const mobile_object& contained) const
		{
			return id == contained.id;
		}

		bool operator==(const nullable<mobile_object>& shared) const
		{
			return id == shared->id;
		}
	};



	class attack_results
	{
	public:
		attack_results() = default;

		int damage_delivered = 0;
		nullable<mobile_object> target_mob;
		nullable<mobile_object> attacking_mob;
	};

	using std::cout;
	using std::pair;
	using std::endl;

	class user_interface
	{
	public:
		void die(nullable<mobile_object> target_mob) const
		{
			cout << target_mob->id << " has died!" << endl;
		}

		void knock_out(nullable<mobile_object> target_mob) const
		{
			cout << target_mob->id << " just got knocked out!" << endl;
		}

		void process_attack(nullable<mobile_object> mob, int damage_roll, nullable<mobile_object> victim) const
		{
			// currently uncalled
			cout << "Process attack..." << endl;
		}

		void receive_damage(nullable<attack_results> results) const
		{
			if (results->attacking_mob == nullptr && results->target_mob == nullptr) return;
			cout << results->attacking_mob->id << " has delivered " << results->damage_delivered << " damage to " << results->target_mob->id << "." << endl;
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

		void attack_misses(nullable<mobile_object> mob, nullable<mobile_object> target_mob) const
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
		vector<nullable<mobile_object>> combatants;

		[[nodiscard]] bool contains(nullable<mobile_object> contained) const
		{
			for (nullable<mobile_object> mob : combatants)
			{
				if (mob == contained) return true;
			}
			return false;
		}
	};


	class battle_results
	{
	public:
		vector<combat_team> groups;
	};
	class mob_ai
	{
	public:
		explicit mob_ai() = default;

		[[nodiscard]] nullable<mobile_object> get_target_for(nullable<mobile_object> item, const vector<nullable<combat_team>>& acgs) const
		{
			for (const nullable<combat_team>& team : acgs)
			{
				if (team->contains(item)) continue;
				for (nullable<mobile_object> combatant : team->combatants)
				{
					if (!combatant->is_dead()) 
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
