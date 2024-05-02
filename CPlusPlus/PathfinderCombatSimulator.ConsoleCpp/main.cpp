#include <iostream>


#include "dice_rolling_tests.h"
#include "combat_helper_tests.h"

using std::cout;
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
    auto diceRollingTests = make_shared<dice_rolling_tests>(*diceManager);

    auto tests = vector<shared_ptr<testClassBase>>();

    tests.push_back(combatHelperTests);
    tests.push_back(diceRollingTests);

    for (int i = 0; i < 100000; ++i)
    {
	    for (auto test : tests)
	    {
            test->run_test();
	    }    	
    }
    cout << "yeah\n";

}



