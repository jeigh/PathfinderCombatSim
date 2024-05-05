
#include "types.h"

using namespace pathfinder_combat_simulator;

auto mob_ai::get_target_for(nullable<mobile_object> item,	const vector<nullable<combat_team>>& acgs) const->nullable<mobile_object>
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




