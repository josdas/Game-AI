#pragma once
#include <vector>
#include <cassert>

class Neuron {
	std::vector<double> weight;

	static double active_function(double x) {
		return 1 / (1 + std::exp(-x));
	}
public:
	explicit Neuron(const std::vector<double>& weight)
		: weight(weight) {}

	double get(std::vector<double> const& data) const {
		assert(weight.size() == data.size());
		double sum = 0;
		for (size_t i = 0; i < weight.size(); i++) {
			sum += weight[i] * data[i];
		}
		return active_function(sum);
	}

	std::vector<double> const& get_weight() const {
		return weight;
	}
};
