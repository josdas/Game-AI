#pragma once
#include "world.h"
#include "point.h"
#include <algorithm>

class World;

enum Direction {
	UP,
	DOWN,
	RIGHT,
	LEFT
};

enum Action_type {
	GO,
	SHOT,
	HIT,
	SLEEP
};

const Point dc[4] = {
	Point(1, 0),
	Point(-1, 0),
	Point(0, 1),
	Point(0, -1),
};

class Unit {
	static const int DAMAGE_SHOT = 4;
	static const int SLEEP_SHOT = 4;
	static const int DAMAGE_HIT = 5;
	static const int SLEEP_HIT = 2;
	static const int SLEEP_GO = 1;
	static const int SLEEP = 1;


	int time_to_next_turn;
	Point cordinate;
	int hp;

public:
	void update();
	bool is_free() const;

	Unit(const Point& cordinate, int hp);

	int get_x() const;
	int get_time() const;
	int get_y() const;
	Point get_cordinate() const;
	int get_hp() const;

	void damage(int d);

	bool go(Direction direction, World* world);
	void shot(Direction direction, World* world);
	void hit(Direction direction, World* world);
	void sleep();
};

inline void Unit::update() {
	time_to_next_turn = std::max(time_to_next_turn - 1, 0);
}

inline bool Unit::is_free() const {
	return time_to_next_turn == 0 && get_hp() > 0;
}

inline Unit::Unit(const Point& cordinate, int hp): time_to_next_turn(0),
                                                   cordinate(cordinate),
                                                   hp(hp) {}

inline int Unit::get_x() const {
	return cordinate.x;
}

inline int Unit::get_time() const {
	return time_to_next_turn;
}

inline int Unit::get_y() const {
	return cordinate.y;
}

inline Point Unit::get_cordinate() const {
	return cordinate;
}

inline int Unit::get_hp() const {
	return hp;
}

inline void Unit::damage(int d) {
	hp = std::max(hp - d, 0);
}

inline bool Unit::go(Direction direction, World* world) {
	time_to_next_turn += SLEEP_GO;
	Point nc = cordinate + dc[direction];
	if (world->inside(nc) && world->is_empty(nc)) {
		cordinate = nc;
		return true;
	}
	return false;
}

inline void Unit::shot(Direction direction, World* world) {
	time_to_next_turn += SLEEP_SHOT;
	Point nc = dc[direction];
	world->shot(cordinate + nc, nc, DAMAGE_SHOT);
}

inline void Unit::hit(Direction direction, World* world) {
	time_to_next_turn += SLEEP_HIT;
	Point nc = dc[direction];
	world->shot(cordinate + nc, nc, DAMAGE_HIT, 1);
}

inline void Unit::sleep() {
	time_to_next_turn += SLEEP;
}


struct Action {
	size_t unit_id;
	Action_type action;
	Direction direction;

	Action(size_t unit_id, Action_type action, Direction direction)
		: unit_id(unit_id),
		action(action),
		direction(direction) {}
};
