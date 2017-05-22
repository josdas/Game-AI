#pragma once
#include <vector>
#include <cassert>
#include "../Random.h"

struct active_function_A {
	static double active_function(double x) {
		return 1 / (1 + std::exp(-x));
	}
};

struct active_function_B {
	static double active_function(double x) {
		return (exp(x) - exp(-x)) / (exp(x) + exp(-x));
	}
};


template<class T>
class Neuron {
	std::vector<double> weight;

	explicit Neuron(const std::vector<double>& weight)
		: weight(weight) {}

public:
	explicit Neuron(size_t s)
		: weight(s) {
		for(auto &v : weight) {
			v = get_rand_double(1);
		}
	}

	double get(std::vector<double> const& data) const {
		assert(weight.size() == data.size());
		double sum = 0;
		for (size_t i = 0; i < weight.size(); i++) {
			sum += weight[i] * data[i];
		}
		return T::active_function(sum);
	}

	std::vector<double> const& get_weight() const {
		return weight;
	}

	std::vector<double>& get_weight() {
		return weight;
	}

	void set(size_t a, double c) {
		weight[a] = c;
	}
};
