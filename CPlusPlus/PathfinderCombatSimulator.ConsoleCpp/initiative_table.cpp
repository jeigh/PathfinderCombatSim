#include <algorithm>

#include "types.h"

using namespace pathfinder_combat_simulator;

void initiative_table::add_roll_by_mob(shared_ptr<mobile_object> addable, int initiative_roll)
{
	shared_ptr<initiative_entry> back_pushable = std::make_shared<initiative_entry>();

	back_pushable->initiative_roll = initiative_roll;
	back_pushable->mob = addable;

	raw.push_back(back_pushable);
}

bool compare_inits(shared_ptr<initiative_entry> a, shared_ptr<initiative_entry> b)
{
	return a->initiative_roll > b->initiative_roll;
}


vector<shared_ptr<mobile_object>> initiative_table::get_ordered_mobs()
{
	vector<std::shared_ptr<mobile_object>> returnable;
	std::ranges::sort(raw, compare_inits);

	returnable.reserve(this->raw.size());
	for (const shared_ptr<initiative_entry>& initiative_entry : this->raw)
	{
		returnable.push_back(initiative_entry->mob);
	}

	return returnable;
}

