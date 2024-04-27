#include <iostream>
#include "test_dice_rolling.h"

using std::cout;
using std::cin;




int main() 
{
    cout << "Press any key to roll dice" << '\n';
    char a = 'a';
    cin >> a;
    int seed = 42;
    srand(seed);
    auto item = new pathfinder_combat_simulator::test_dice_rolling();
    while (true)
    {
        item->run_test();
	}



}



