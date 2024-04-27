#pragma once

#include "DieRoll.h"
#include <vector>
#include "DiceManager.h"

using std::vector;

namespace PathfinderCombatSimulator
{
    class DiceManager
    {


    public:
        int AddRolls(const vector<DieRoll>& dice);

    private:
        int Roll(int dieSize);
    };
}

