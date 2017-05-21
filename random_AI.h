#pragma once
#include "strategy.h"

class Random_AI : public Strategy {

public:
	explicit Random_AI(size_t id_unit)
		: Strategy(id_unit) {}

	Action next_action(World* world) const override {
		return Action(
			unit_id, 
			static_cast<Action_type>(rand() % 4), 
			static_cast<Direction>(rand() % 4)
		);
	}
};