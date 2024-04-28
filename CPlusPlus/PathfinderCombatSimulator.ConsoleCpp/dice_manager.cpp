#include <cstdlib>
#include "types.h"

namespace pathfinder_combat_simulator
{

	[[nodiscard]] int dice_manager::add_rolls(const vector<die_roll> dice) const
	{
        int returnable = 0;

        for (die_roll d : dice)
        {
            int rollsLeft = d.die_count;
            while (rollsLeft > 0)
            {
                if (d.die_size > 0) {
                    auto justRolled = roll(d.die_size);
                    returnable += justRolled;
                }
                rollsLeft--;
            }
        }
        return returnable;
	}

	[[nodiscard]] int dice_manager::roll(const int die_size) const
	{
        return rand() % die_size + 1;
    }


}

