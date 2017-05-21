#pragma once
#include "strategy.h"
#include <algorithm>
#include "../Game/unit.h"
#include "../GameConst.h"

class Neural_AI : public Strategy {
	Neural_network neural_network;

	std::vector<double> get_data(World* world) const {
		std::vector<double> data;
		size_t enemy_id = unit_id ^ 1;
		auto units = world->get_units();
		for (size_t i : {unit_id, enemy_id}) {
			data.push_back(units[i]->get_hp() / START_HP); //Normalize
			data.push_back(units[i]->get_x() / MAX_H);
			data.push_back(units[i]->get_y() / MAX_W);
			data.push_back(units[i]->get_time());
		}
		data.push_back((units[unit_id]->get_x() - units[enemy_id]->get_x()) / MAX_H);
		data.push_back((units[unit_id]->get_y() - units[enemy_id]->get_y()) / MAX_W);
		data.push_back(units[unit_id]->get_x() == units[enemy_id]->get_x());
		data.push_back(units[unit_id]->get_y() == units[enemy_id]->get_y());
		data.push_back(units[unit_id]->get_x() < units[enemy_id]->get_x());
		data.push_back(units[unit_id]->get_y() < units[enemy_id]->get_y());
		return data;
	}

public:
	explicit Neural_AI(size_t id_unit, Neural_network const& neural_network)
		: Strategy(id_unit),
		  neural_network(neural_network) {}

	Action next_action(World* world) const override {
		std::vector<double> data = get_data(world);
		std::vector<double> prob = neural_network.get(data);
		auto ind = max_element(prob.begin(), prob.end()) - prob.begin();
		return Action(
			unit_id,
			static_cast<Action_type>(ind / 4),
			static_cast<Direction>(ind % 4)
		);
	}
};
