#pragma once

#include "types.h"
#include <memory>



using std::unique_ptr;
using pathfinder_combat_simulator::dice_manager;
using pathfinder_combat_simulator::die_roll;

namespace pathfinder_combat_simulator
{
	class test_dice_rolling
	{
	public:
		void run_test() const;

	private:
	    int die_size_ = 3;
	    int die_count_ = 6;

		[[nodiscard]] unique_ptr<dice_manager> make_dice_manager() const;
		[[nodiscard]] unique_ptr<die_roll> make_die_roll(int, int) const;
	};
}