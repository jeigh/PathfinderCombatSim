#include "test_dice_rolling.h"

using std::cout;
using std::unique_ptr;
using std::make_unique;

namespace pathfinder_combat_simulator
{

    unique_ptr<dice_manager> test_dice_rolling::make_dice_manager() const
    {
        return make_unique<dice_manager>();
    }

    unique_ptr<die_roll> test_dice_rolling::make_die_roll(int count, int size) const
    {
	    return make_unique<die_roll>(count, size);
    }
    
    void test_dice_rolling::run_test() const
    {
        auto dice_manager = make_dice_manager();

        auto die1 = make_die_roll(die_count_, die_size_);
        auto die2 = make_die_roll(die_count_, die_size_);

        vector dice = { *die1, *die2 };

        int result = dice_manager->add_rolls(dice);
        cout << "Rolled Total: " << result << '\n';
    }
}