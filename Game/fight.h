#pragma once
#include "world.h"
#include "../AI/strategy.h"
#include "../GameConst.h"

class Fight {
	World* world;
	Strategy* first;
	Strategy* second;
	int turn;

public:

	Fight(World* world, Strategy* first, Strategy* second);

	void next_turn();
	void play();
};

inline Fight::Fight(World* world, Strategy* first, Strategy* second): world(world),
                                                                      first(first),
                                                                      second(second),
                                                                      turn(0) {}

inline void Fight::next_turn() {
	if (turn % 2 == 0) {
		world->update_unit(first->get_next_action(world));
	}
	else {
		world->update_unit(second->get_next_action(world));
	}
	turn++;
}

inline void Fight::play() {
	for (int i = 0; i < MAX_TIME_GAME; i++) {
		next_turn();
	}
}
