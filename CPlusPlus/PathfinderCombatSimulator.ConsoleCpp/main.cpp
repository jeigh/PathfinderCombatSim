#include <iostream>
#include "test_dice_rolling.h"

using std::cout;
using std::cin;
using std::make_unique;
using std::string;


void do_work(const dice_manager& roller)
{
	auto null_attack = make_unique<pathfinder_combat_simulator::attack>();
	auto mob = make_unique<pathfinder_combat_simulator::mobile_object>("a randomized mob", roller.roll(100) - 20, roller.roll(100) , 10, 4, *null_attack);

	auto unit = make_unique<pathfinder_combat_simulator::combat_helper>();

	string status = unit->is_dead(*mob) ? " is dead" : " still lives";
	status += unit->is_unconcious(*mob) ? ", but is unconcious!\n" : "!\n";

	cout << "At " << mob->current_hit_points << " hit points, " << mob->id << status;
}

int main() 
{
    cout << "Press any key to begin stress test" << '\n';
    char a = 'a';
    cin >> a;
    int seed = 42;
    srand(seed);
    const auto roller = make_unique<dice_manager>();

    while (true)
    {
        do_work(*roller);
	}



}



