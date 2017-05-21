#pragma once
#include "world.h"
#include "unit.h"

struct Strategy {
	size_t unit_id;

	explicit Strategy(size_t id_unit)
		: unit_id(id_unit) {}

	virtual ~Strategy() = default;

	Action get_next_action(World* world) const {
		auto action = next_action(world);
		std::cout << unit_id  << ' ' 
		<< action.action  << ' ' 
		<< action.direction << '\n';
		return action;
	}
protected:
	virtual Action next_action(World* world) const = 0;
};