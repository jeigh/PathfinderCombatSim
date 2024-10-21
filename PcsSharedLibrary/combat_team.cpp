#include "types.h"

using namespace pathfinder_combat_simulator;

[[nodiscard]] bool combat_team::contains(shared_ptr<mobile_object> contained) const
{
	for (shared_ptr<mobile_object> mob : combatants)
	{
		if (mob == contained) return true;
	}
	return false;
}
