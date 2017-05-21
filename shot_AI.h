#pragma once
#include "strategy.h"

class Shot_AI : public Strategy {

public:
	explicit Shot_AI(size_t id_unit)
		: Strategy(id_unit) {}

	Action next_action(World* world) const override {
		int enemy = unit_id ^ 1; // if there are two units

		return Action(
			unit_id,
			static_cast<Action_type>(rand() % 4),
			static_cast<Direction>(rand() % 4)
		);
	}
};
