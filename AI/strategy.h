#pragma once
#include "../Game/unit.h"

struct Strategy {
	size_t unit_id;

	explicit Strategy(size_t unit_id);
	virtual ~Strategy() = default;

	Action get_next_action(World* world) const;
protected:
	virtual Action next_action(World* world) const = 0;
};

inline Strategy::Strategy(size_t unit_id): unit_id(unit_id) {}

inline Action Strategy::get_next_action(World* world) const {
	auto action = next_action(world);
	/*std::cout << unit_id  << ' ' 
	<< action.action  << ' ' 
	<< action.direction << '\n';*/
	return action;
}
