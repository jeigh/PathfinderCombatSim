#include <iostream>

#include "types.h"
#include <mutex>
#include <thread>

using namespace pathfinder_combat_simulator;

[[nodiscard]] int dice_manager::add_rolls(const vector<nullable<die_roll>> dice) const
{
	int returnable = 0;

	for (nullable<die_roll> d : dice)
	{
		int rollsLeft = d->die_count;
		while (rollsLeft > 0)
		{
			if (d->die_size > 0) {
				auto justRolled = roll(d->die_size);
				returnable += justRolled;
			}
			rollsLeft--;
		}
	}
	return returnable;

}

[[nodiscard]] int dice_manager::roll(const int die_size) const
{
	static std::mutex your_mutex;
	your_mutex.lock();
	auto returnable = rand() % die_size + 1;
	//std::cout << "rolled " << returnable << " on a d" << die_size << std::endl;
	your_mutex.unlock();
	return returnable;
}

