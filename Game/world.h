#pragma once
#include "field.h"
#include <vector>
#include "../GameConst.h"

struct Action;
class Unit;

class World {
	Field field;
	std::vector<Unit*> units;
public:
	World(const Field& field, Unit* first, Unit* second);

	bool inside(Point p) const;
	int get_h() const;
	int get_w() const;
	std::vector<Unit*> const& get_units() const;
	Unit* get_cell(Point p) const;

	bool is_empty(Point p) const;
	Unit* ray_cast(Point start, Point dp, int time) const;
	void shot(Point start, Point dp, int damage, int time = MAX_TIME_GAME);

	void update_unit(Action action);
};
