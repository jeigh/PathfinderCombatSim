
#include "types.h"

using namespace pathfinder_combat_simulator;

auto mob_ai::get_target_for(shared_ptr<mobile_object> item,	const vector<shared_ptr<combat_team>>& acgs) const->shared_ptr<mobile_object>
{
	for (const shared_ptr<combat_team>& team : acgs)
	{
		if (team->contains(item)) continue;
		for (shared_ptr<mobile_object> combatant : team->combatants)
		{
			if (!combatant->is_dead())
				return combatant;
		}
	}
	return nullptr;
}




