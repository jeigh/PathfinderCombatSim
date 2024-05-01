#pragma once

#include <memory>
#include <iostream>
#include "types.h"

using std::make_unique;
using std::cout;

namespace pathfinder_combat_simulator
{
	class combat_helper_tests : public testClassBase
	{
	private:
	    dice_manager _roller;

	public:
		explicit combat_helper_tests(const dice_manager& roller) : _roller(roller) {}
		void run_test() const override;
	};
}

