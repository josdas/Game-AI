#pragma once
#include "strategy.h"
#include "../Game/unit.h"

class Random_AI : public Strategy {

public:
	explicit Random_AI(size_t id_unit);

	Action next_action(World* world) const override;
};

inline Random_AI::Random_AI(size_t id_unit): Strategy(id_unit) {}

inline Action Random_AI::next_action(World* world) const {
	return Action(
		unit_id,
		static_cast<Action_type>(rand() % 4),
		static_cast<Direction>(rand() % 4)
	);
}
