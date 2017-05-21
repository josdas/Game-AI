#pragma once
#include "world.h"
#include "strategy.h"

class Fight {
	static const int MAX_TIME_GAME = 100;

	World* world;
	Strategy* first;
	Strategy* second;
	int turn;
public:

	Fight(World* world, Strategy* first, Strategy* second)
		: world(world),
		  first(first),
		  second(second),
		  turn(0) {}

	void next_turn() {
		if (turn % 2 == 0) {
			world->update_unit(first->get_next_action(world));
		}
		else {
			world->update_unit(second->get_next_action(world));
		}
		turn++;
	}

	void play() {
		for (int i = 0; i < MAX_TIME_GAME; i++) {
			next_turn();
		}
	}
};
