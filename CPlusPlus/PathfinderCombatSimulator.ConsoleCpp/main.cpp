#include <iostream>


#include "test_dice_rolling.h"
#include "combat_helper_tests.h"

using std::cout;
using std::cin;
using std::make_unique;
using std::string;


using namespace pathfinder_combat_simulator;


int main() 
{
    int seed = 42;
    srand(seed);
    cout << "Hit a key to begin...\n";

    if (!getchar()) return 0;

	auto diceManager = make_unique<dice_manager>();
    auto combatHelperTests = make_shared<combat_helper_tests>(*diceManager);
    auto diceRollingTests = make_shared<test_dice_rolling>(*diceManager);

    auto tests = vector<shared_ptr<testClassBase>>();

    tests.push_back(combatHelperTests);
    tests.push_back(diceRollingTests);

    for (int i = 0; i < 100000; ++i)
    {
	    for (const shared_ptr<testClassBase>& test : tests)
	    {
            test->run_test();
	    }
    	
    }
    cout << "yeah\n";

}



