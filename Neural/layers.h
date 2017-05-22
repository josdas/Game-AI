#pragma once
#include "neuron.h"

template<class T>
class Layer {
	std::vector<std::vector<double> > neurons;

public:
	explicit Layer(const std::vector<std::vector<double>>& neurons)
		: neurons(neurons) {}

	explicit Layer(size_t s, size_t past) {
		neurons.resize(s);
		for(size_t i = 0; i < s; i++) {
			neurons[i].resize(past);
		}
	}

	std::vector<double> get(std::vector<double> const& data) const {
		std::vector<double> result(neurons.size());
		for(size_t i = 0; i < neurons.size(); i++) {
			double &sum = result[i];
			#pragma omp parallel for reduction(+:sum)
			for(size_t j = 0; j < data.size(); j++) {
				sum += data[j] * neurons[i][j];
			}
			#pragma omp barrier
			sum = T::active_function(sum);
		}
		return result;
	}

	std::vector<std::vector<double> > const& get_neurons_weight() const {
		return neurons;
	}

	std::vector<double> get_coefficient() const {
		std::vector<double> result;
		for (size_t i = 0; i < neurons.size(); i++) {
			for(auto v : neurons[i]) {
				result.push_back(v);
			}
		}
		return result;
	}

	void set(size_t a, size_t b, double x) {
		neurons[a][b] = x;
	}

	void resize(size_t n) {
		for(auto &v : neurons) {
			v.resize(n);
		}
	}
};
