#pragma once

#include "types.h"
#include <memory>
#include <iostream>



using std::unique_ptr;
using pathfinder_combat_simulator::dice_manager;
using pathfinder_combat_simulator::die_roll;
using std::make_unique;
using std::make_shared;
using std::shared_ptr;
using std::cout;


namespace pathfinder_combat_simulator
{
	class dice_rolling_tests : public testClassBase
	{
	public:
		explicit dice_rolling_tests(const dice_manager& dice_manager) :
			_dice_manager(dice_manager) {}
		
		void run_test() const override;


	private:
	    int die_size_ = 3;
	    int die_count_ = 6;
		dice_manager _dice_manager;

		[[nodiscard]] shared_ptr<die_roll> make_die_roll(int count, int size) const
		{
			return make_unique<die_roll>(count, size);
		}
	};
}