#include "types.h"
#include <iostream>
#include <algorithm>

using std::cout;
using std::pair;
using std::endl;

using namespace pathfinder_combat_simulator;

void user_interface::output_aggregates(shared_ptr<battle> the_battle, unordered_map<string, int> const& winners) const
{
	if (_level < output_level::low) return;
	_ui_mutex->lock();

	// currently uncalled
	for (const pair<string, int> winner : winners)
	{
		cout << "Battle " << the_battle->id << ": " << winner.first << " Wins : " << winner.second << endl;
	}

	_ui_mutex->unlock();
}

void user_interface::die(shared_ptr<battle> the_battle, shared_ptr<mobile_object> target_mob) const
{
	if (_level < output_level::low) return;
	_ui_mutex->lock();

	cout << "Battle " << the_battle->id << ": " << target_mob->id << " has died!" << endl;

	_ui_mutex->unlock();
}

void user_interface::attack_misses(shared_ptr<battle> the_battle, shared_ptr<mobile_object> mob, const shared_ptr<mobile_object> target_mob) const
{
	if (_level < output_level::medium) return;
	_ui_mutex->lock();

	cout << "Battle " << the_battle->id << ": " << mob->id << " attempted to hit " << target_mob->id << "  but missed." << endl;

	_ui_mutex->unlock();
}

void user_interface::receive_damage(shared_ptr<battle> the_battle, shared_ptr<attack_results> results) const
{
	if (_level < output_level::medium) return;
	_ui_mutex->lock();

	if (results->attacking_mob == nullptr && results->target_mob == nullptr)
	{
		_ui_mutex->unlock();
		return;
	}

	cout << "Battle " << the_battle->id << ": " << results->attacking_mob->id << " has delivered " << results->damage_delivered << " damage to " << results->target_mob->id << "." << endl;

	_ui_mutex->unlock();
}


void user_interface::knock_out(shared_ptr<battle> the_battle, shared_ptr<mobile_object> target_mob) const
{
	if (_level < output_level::high) return;
	_ui_mutex->lock();

	cout << "Battle " << the_battle->id << ": " << target_mob->id << " just got knocked out!" << endl;

	_ui_mutex->unlock();
}

void user_interface::process_attack(shared_ptr<battle> the_battle, shared_ptr<mobile_object> mob, int damage_roll, shared_ptr<mobile_object> victim) const
{
	// currently uncalled
	if (_level < output_level::high) return;
	_ui_mutex->lock();

	cout << "Battle " << the_battle->id << ": Process attack..." << endl;

	_ui_mutex->unlock();
}


void user_interface::round_ends(shared_ptr<battle> the_battle, int turn_id) const
{
	if (_level < output_level::max) return;
	_ui_mutex->lock();

	cout << "Battle " << the_battle->id << ": Round " << turn_id << " ends." << endl;

	_ui_mutex->unlock();
}

void user_interface::round_starts(shared_ptr<battle> the_battle, int turn_id) const
{
	if (_level < output_level::max) return;
	_ui_mutex->lock();

	cout << "Battle " << the_battle->id << ": Round " << turn_id << " starts." << endl;

	_ui_mutex->unlock();
}



