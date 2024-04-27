#include "DiceManager.h"
#include "DieRoll.h"
#include <cstdlib>

namespace PathfinderCombatSimulator
{

	int DiceManager::AddRolls(const vector<DieRoll>& dice)
	{
        int returnable = 0;

        for (DieRoll d : dice)
        {
            int rollsLeft = d.DieCount;
            while (rollsLeft > 0)
            {
                if (d.DieSize > 0) {
                    auto justRolled = Roll(d.DieSize);
                    returnable += justRolled;
                }
                rollsLeft--;
            }
        }
        return returnable;
	}

    int DiceManager::Roll(int dieSize)
    {
        return rand() % dieSize + 1;
    }


}

