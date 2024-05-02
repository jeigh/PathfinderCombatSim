#include "combat_helper_tests.h"
#include <iostream>
#include <memory>
#include "types.h"

using std::make_unique;
using std::cout;
using std::make_shared;

namespace pathfinder_combat_simulator
{
	void combat_helper_tests::run_test() const 
	{
		auto null_attack = make_unique<attack>();
		auto mob = make_shared<mobile_object>("a randomized mob", _roller.roll(100) - 20, _roller.roll(100), 10, 4, nullptr);

		auto unit = make_unique<combat_helper>();

		std::pmr::string status = unit->is_dead(mob) ? " is dead" : " still lives";
		status += unit->is_unconcious(mob) ? ", but is unconcious!\n" : "!\n";

		cout << "At " << mob->current_hit_points << " hit points, " << mob->id << status;
	}


}
