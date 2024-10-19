#include "types.h"
#include <fstream>
#include <memory>
#include <iostream>

using namespace pathfinder_combat_simulator;

void data_access::persist_attack_results(shared_ptr<attack_request> request, attack_outcome outcome)
{
	//_db_mutex->lock();

	if (!_attackfile.is_open()) 
	{
		_attackfile.open(this->_attack_scenario_file_name, std::ios::out);

		_attackfile
			<< "attack_bonus,"
			<< "armor_class,"
			<< "minimum_crit,"
			<< "unmodified_attack_roll,"
			<< "attackers_str_or_dex_modifier,"
			<< "crit_confirmation_roll,"
			<< "attack_outcome"
			<< std::endl;

	}
		

	_attackfile 
		<< request->attack_bonus << ","
		<< request->armor_class << ","
		<< request->minimum_crit << ","
		<< request->unmodified_attack_roll << ","
		<< request->str_modifier << ","
		<< request->crit_confirmation_roll << ","
		<< static_cast<int>(outcome) 
		<< std::endl;

	//_db_mutex->unlock();

}

void data_access::persist_damage_results(int damage_dice_count, float statistical_damage_mean, shared_ptr<damage_request> request, float expected_result)
{
	//_db_mutex->lock();

	if (!_damagefile.is_open()) 
	{
		_damagefile.open(this->_damage_scenario_file_name, std::ios::out);

		_damagefile
			<< "attack_outcome,"
			<< "damage_dice_count,"
			<< "statistical_damage_mean,"
			<< "crit_multiplier,"
			<< "attribute_modifier,"
			<< "estimated_damage"
			<< std::endl;

	}

	_damagefile
		<< static_cast<int>(request->this_attack_outcome) << ","
		<< damage_dice_count << ","
		<< statistical_damage_mean << ","
		<< request->crit_multiplier << ","
		<< request->attribute_modifier << ","
		<< expected_result
		<< std::endl;

	//_db_mutex->unlock();
}


