#include <iostream>

#include "types.h"

using std::cout;
using std::pair;

namespace pathfinder_combat_simulator 
{
	void user_interface::die(mobile_object target_mob) const
	{
		
	}
	void user_interface::flush() const
	{
		
	}
	void user_interface::knock_out(mobile_object target_mob) const
	{
		
	}
	void user_interface::process_attack(mobile_object mob, int damage_roll, mobile_object victim) const
	{
		
	}
	void user_interface::receive_damage(shared_ptr<attack_results> results) const
	{
		
	}
	void user_interface::round_ends(int turn_id) const
	{
		
	}
	void user_interface::round_starts(int turn_id) const
	{
		
	}

	void user_interface::output_aggregates(unordered_map<string, int> const& winners) const
	{
		for (const pair<string, int> winner : winners)
		{
			cout << winner.first << " Wins : " << winner.second << '\n';
		}
	}

	void user_interface::attack_misses(shared_ptr<mobile_object> mob, shared_ptr<mobile_object> target_mob) const
	{
		
	}
}


