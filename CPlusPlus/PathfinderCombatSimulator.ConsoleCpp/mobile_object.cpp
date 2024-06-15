#include "types.h"

using namespace pathfinder_combat_simulator;

[[nodiscard]] bool mobile_object::is_dead() const
{
	return current_hit_points <= -10;
}

[[nodiscard]] bool mobile_object::is_unconcious() const
{
	return current_hit_points > -10 && current_hit_points <= 0;
}

[[nodiscard]] bool mobile_object::is_prone() const
{
	return prone;
}

[[nodiscard]] bool mobile_object::operator==(const mobile_object& contained) const
{
	return id == contained.id;
}

[[nodiscard]] bool mobile_object::operator==(const shared_ptr<mobile_object>& shared) const
{
	return id == shared->id;
}
