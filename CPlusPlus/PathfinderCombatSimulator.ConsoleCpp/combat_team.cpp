#include "types.h"

using namespace pathfinder_combat_simulator;

[[nodiscard]] bool combat_team::contains(nullable<mobile_object> contained) const
{
	for (nullable<mobile_object> mob : combatants)
	{
		if (mob == contained) return true;
	}
	return false;
}
