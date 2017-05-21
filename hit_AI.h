#pragma once
#include "strategy.h"

class Hit_AI : public Strategy {
	Direction get_direction(Point p) const {
		if (abs(p.x) < abs(p.y) || p.x == 0) {
			if (p.y > 0) {
				return LEFT;
			}
			if (p.y < 0) {
				return RIGHT;
			}
		}
		if (p.x < 0) {
			return UP;
		}
		if (p.x > 0) {
			return DOWN;
		}
		return UP;
	}

public:
	explicit Hit_AI(size_t id_unit)
		: Strategy(id_unit) {}

	Action next_action(World* world) const override {
		int enemy_id = unit_id ^ 1; // if there are two units
		Point my_cordinate = world->get_units()[unit_id]->get_cordinate();
		Point en_cordinate = world->get_units()[enemy_id]->get_cordinate();
		Point dl = my_cordinate - en_cordinate;
		if (my_cordinate.x == en_cordinate.x || my_cordinate.y == en_cordinate.y) {

			return Action(
				unit_id,
				dl.length() == 1 ? HIT : SHOT,
				get_direction(dl)
			);
		}
		return Action(
			unit_id,
			GO,
			get_direction(dl)
		);
	}
};
