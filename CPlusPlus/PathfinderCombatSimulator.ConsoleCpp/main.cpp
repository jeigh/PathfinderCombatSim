#include <iostream>
#include "TestDiceRolling.h"

using std::cout;
using std::cin;




int main() 
{
    cout << "Press any key to roll dice" << '\n';
    char a = 'a';
    cin >> a;
    int seed = 42;
    srand(seed);
    auto item = new PathfinderCombatSimulator::TestDiceRolling();
    while (true)
    {
        item->RunTest();
	}



}



