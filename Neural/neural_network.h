#pragma once
#include <vector>
#include <cmath>
#include "layers.h"

class Neural_network {
	std::vector<Layer> layers;

public:
	explicit Neural_network(const std::vector<Layer>& layers)
		: layers(layers) {}

	std::vector<double> get(std::vector<double> const& data) const {
		std::vector<double> result = data;
		for(size_t i = 0; i < layers.size(); i++) {
			result = layers[i].get(result);
		}
		return result;
	}

	std::vector<std::vector<std::vector<double> > > get_neurons_weight() const {
		std::vector<std::vector<std::vector<double> > > result(layers.size());
		for (size_t i = 0; i < layers.size(); i++) {
			result[i] = layers[i].get_neurons_weight();
		}
		return result;
	}
};
