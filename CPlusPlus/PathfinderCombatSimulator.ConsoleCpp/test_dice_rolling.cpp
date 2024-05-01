#include "test_dice_rolling.h"
#include <iostream>

using std::cout;

namespace pathfinder_combat_simulator
{
    void test_dice_rolling::run_test() const 
    {
        auto die1 = make_die_roll(die_count_, die_size_);
        auto die2 = make_die_roll(die_count_, die_size_);

        vector<die_roll> dice = {*die1, *die2};

        int result = _dice_manager.add_rolls(dice);
        cout << "Rolled Total: " << result << '\n';
    }
}