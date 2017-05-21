#pragma once
#include <vector>
#include <cmath>
#include "layers.h"

class Neural_network {
	std::vector<Layer> layers;

public:
	std::vector<double> get(std::vector<double> const& data) const {
		std::vector<double> result = data;
		for(size_t i = 0; i < layers.size(); i++) {
			result = layers[i].get(result);
		}
		return result;
	}
};
