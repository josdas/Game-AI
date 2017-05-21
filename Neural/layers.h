#pragma once
#include "neuron.h"

class Layer {
	std::vector<Neuron> neurons;
public:
	explicit Layer(const std::vector<Neuron>& neurons)
		: neurons(neurons) {}

	std::vector<double> get(std::vector<double> const& data) const {
		std::vector<double> result(neurons.size());
		for(size_t i = 0; i < neurons.size(); i++) {
			result[i] = neurons[i].get(data);
		}
		return result;
	}

	std::vector<std::vector<double> > get_neurons_weight() const {
		std::vector<std::vector<double> > result(neurons.size());
		for (size_t i = 0; i < neurons.size(); i++) {
			result[i] = neurons[i].get_weight();
		}
		return result;
	}
};
