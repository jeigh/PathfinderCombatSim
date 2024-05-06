#include "types.h"

using namespace pathfinder_combat_simulator;

[[nodiscard]] vector<std::shared_ptr<mobile_object>> battle::get_combatants() const
{
	vector<std::shared_ptr<mobile_object>> returnable;
	for (const std::shared_ptr<combat_team>& combat_team : combat_teams)
	{
		for (const std::shared_ptr<mobile_object>& mob : combat_team->combatants)
		{
			returnable.push_back(mob);
		}
	}
	return returnable;
}

void battle::round_ends()
{
	for (nullable<mobile_object> mob : get_combatants())
	{
		mob->number_of_previous_attacks_made_this_turn = 0;
	}
}
