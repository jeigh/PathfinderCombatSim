#pragma once

#include <iostream>
#include "DiceManager.h"

using std::unique_ptr;
using PathfinderCombatSimulator::DiceManager;
using PathfinderCombatSimulator::DieRoll;

namespace PathfinderCombatSimulator
{
	class TestDiceRolling
	{

	public:
		void RunTest();

	private:
	    int _dieSize = 3;
	    int _dieCount = 6;

	    unique_ptr<DiceManager> MakeDiceManager();
	    auto MakeDieRoll(int, int);
	};
}