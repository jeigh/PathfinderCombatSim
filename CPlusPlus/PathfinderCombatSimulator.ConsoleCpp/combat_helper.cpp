#include "types.h"

namespace pathfinder_combat_simulator
{
	[[nodiscard]] auto combat_helper::is_dead(shared_ptr<mobile_object> mob) const -> bool
	{
		return mob->current_hit_points <= -10;
	}

	[[nodiscard]] auto combat_helper::is_unconcious(shared_ptr<mobile_object> mob) const-> bool
	{
		return (mob->current_hit_points > -10 && mob->current_hit_points <= 0);
	}
}
