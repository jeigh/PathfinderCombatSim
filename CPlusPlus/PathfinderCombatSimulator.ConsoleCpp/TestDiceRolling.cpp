#include "TestDiceRolling.h"

using std::cout;
using std::unique_ptr;
using std::make_unique;

using namespace PathfinderCombatSimulator;

unique_ptr<DiceManager> TestDiceRolling::MakeDiceManager()
{
    return make_unique<DiceManager>();
}

auto TestDiceRolling::MakeDieRoll(int count, int size)
{
    return make_unique<DieRoll>(_dieCount, _dieSize);
}

void TestDiceRolling::RunTest()
{
    unique_ptr<DiceManager> dice_manager = MakeDiceManager();
    auto die1 = make_unique<DieRoll>(_dieCount, _dieSize);
    auto die2 = make_unique<DieRoll>(_dieCount, _dieSize);
    vector dice = { *die1, *die2 };

    int result = dice_manager->AddRolls(dice);
    cout << "Rolled Total: " << result << '\n';
}



