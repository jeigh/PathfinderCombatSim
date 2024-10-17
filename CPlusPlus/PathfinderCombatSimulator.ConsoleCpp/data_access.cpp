#include "types.h"
#include <fstream>
#include <memory>
#include <iostream>

using namespace pathfinder_combat_simulator;

data_access::~data_access() 
{
	close_connection();
}

void data_access::initialize_connection() 
{
	if (!_file.is_open())
	_file.open(this->_attack_scenario_file_name, std::ios::app);
}

void data_access::close_connection() 
{
	if (_file.is_open()) _file.close();
}


void data_access::insert_attack_scenarios(vector<shared_ptr<attack_scenario>> scenarios)
{
	_db_mutex->lock();

	if (_file.is_open())
	{
		for (auto scenario : scenarios)
		{
			repeatable_insert_logic(scenario);
		}
	}

	_db_mutex->unlock();
}

void data_access::repeatable_insert_logic(shared_ptr<attack_scenario> scenario)
{
	_file << scenario->attack_bonus << ","
		<< scenario->armor_class << ","
		<< scenario->minimum_crit << ","
		<< scenario->crit_multiplier << ","
		<< scenario->unmodified_attack_roll << ","
		<< scenario->damage_dice_count << ","
		<< scenario->mean_damage_per_die << ","
		<< scenario->expected_result << "\n";
}

void data_access::insert_attack_scenario(shared_ptr<attack_scenario> scenario) {
	_db_mutex->lock();

	if (_file.is_open()) 
		repeatable_insert_logic(scenario);

	_db_mutex->unlock();
}