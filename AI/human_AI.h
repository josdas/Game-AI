#pragma once
#include "strategy.h"
#include "../Game/unit.h"
#include <iostream>

class Human_AI : public Strategy {
public:
	explicit Human_AI(size_t id_unit);

	Action next_action(World* world) const override;
};

inline Human_AI::Human_AI(size_t id_unit): Strategy(id_unit) {}

inline Action Human_AI::next_action(World* world) const {
	char action, direction;
	Action_type a;
	Direction d;
	while (true) {
		std::cin >> action >> direction;
		bool ok = true;
		switch (action) {
		case 'q':
			a = SLEEP;
			break;
		case 'h':
			a = HIT;
			break;
		case 'g':
			a = GO;
			break;
		case 's':
			a = SHOT;
			break;
		default:
			ok = false;
		}
		switch (direction) {
		case 'w':
			d = UP;
			break;
		case 'a':
			d = LEFT;
			break;
		case 'd':
			d = RIGHT;
			break;
		case 's':
			d = DOWN;
			break;
		default:
			ok = false;
		}
		if (ok) {
			std::cerr << "ok " << action << direction << '\n';
			break;
		}
		else {
			std::cerr << "fail\n";
		}
	}
	return Action(
		unit_id,
		a,
		d
	);
}
