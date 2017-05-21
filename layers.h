#pragma once
#include "neuron.h"

class Layer {
	std::vector<Neuron> neurons;
public:
	std::vector<double> get(std::vector<double> const& data) const {
		std::vector<double> result(neurons.size());
		for(size_t i = 0; i < neurons.size(); i++) {
			result[i] = neurons[i].get(data);
		}
		return result;
	}
};
