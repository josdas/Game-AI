#include "world.h"
#include "unit.h"

World::World(const Field& field, Unit* first, Unit* second): field(field) {
	units.push_back(first);
	units.push_back(second);
}

bool World::inside(Point p) const {
	return field.inside(p);
}

int World::get_h() const {
	return field.h;
}

int World::get_w() const {
	return field.w;
}

std::vector<Unit*> const& World::get_units() const {
	return units;
}

Unit* World::get_cell(Point p) const {
	for (auto unit : units) {
		if (unit->get_cordinate() == p) {
			return unit;
		}
	}
	return nullptr;
}

bool World::is_empty(Point p) const {
	return get_cell(p) == nullptr;
}

Unit* World::ray_cast(Point start, Point dp, int time) const {
	while (inside(start) && time > 0) {
		auto unit = get_cell(start);
		if (unit != nullptr) {
			return unit;
		}
		start = start + dp;
		time--;
	}
	return nullptr;
}

void World::shot(Point start, Point dp, int damage, int time) {
	auto target = ray_cast(start, dp, time);
	if (target != nullptr) {
		target->damage(damage);
	}
}

void World::update_unit(Action action) {
	Unit* unit = units[action.unit_id];
	unit->update();
	if (unit->is_free()) {
		switch (action.action) {
		case GO:
			unit->go(action.direction, this);
			break;
		case SHOUT:
			unit->shot(action.direction, this);
			break;
		case HIT:
			unit->hit(action.direction, this);
			break;
		case SLEEP:
			unit->sleep();
			break;
		default:
			throw std::runtime_error("Incorrect switch!");
		}
	}
}
