#pragma once
#include "neuron.h"

template<class T>
class Layer {
	std::vector<Neuron<T> > neurons;

public:
	explicit Layer(const std::vector<Neuron<T>>& neurons)
		: neurons(neurons) {}

	explicit Layer(size_t s, size_t past) {
		for(size_t i = 0; i < s; i++) {
			neurons.emplace_back(past);
		}
	}

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

	std::vector<double> get_coefficient() const {
		std::vector<double> result;
		for (size_t i = 0; i < neurons.size(); i++) {
			auto temp = neurons[i].get_weight();
			for(auto v : temp) {
				result.push_back(v);
			}
		}
		return result;
	}

	void set(size_t a, size_t b, double x) {
		neurons[a].set(b, x);
	}

	void resize(size_t n) {
		for(auto &v : neurons) {
			v.get_weight().resize(n);
		}
	}
};
