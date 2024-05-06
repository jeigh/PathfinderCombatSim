#include <algorithm>

#include "types.h"

using namespace pathfinder_combat_simulator;

void initiative_table::add_roll_by_mob(nullable<mobile_object> addable, int initiative_roll) 
{
	nullable<initiative_entry> back_pushable = std::make_shared<initiative_entry>();

	back_pushable->initiative_roll = initiative_roll;
	back_pushable->mob = addable;

	raw.push_back(back_pushable);
}

bool compare_inits(nullable<initiative_entry> a, nullable<initiative_entry> b)
{
	return a->initiative_roll > b->initiative_roll;
}


vector<nullable<mobile_object>> initiative_table::get_ordered_mobs()
{
	vector<std::shared_ptr<mobile_object>> returnable;

	std::sort(begin(this->raw), std::end(this->raw), compare_inits);


	returnable.reserve(this->raw.size());
	for (const nullable<initiative_entry>& initiative_entry : this->raw)
	{
		returnable.push_back(initiative_entry->mob);
	}

	return returnable;
}

