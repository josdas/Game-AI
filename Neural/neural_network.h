#pragma once
#include <vector>
#include "layers.h"
#include "active_layer.h"
#include <ostream>
#include "../my_stream.h"

struct Neural_coef {
	enum class layer_type {

	};
	friend std::ostream& operator<<(std::ostream& out, const Neural_coef& a);
	std::vector<size_t> layers_size;
	std::vector<double> coefficient;
	std::vector<layer_type> layers_type;

	Neural_coef(const std::vector<size_t>& layers_size, const std::vector<double>& coefficient);
};

inline std::ostream& operator<<(std::ostream& out, const Neural_coef& a) {
	out << a.layers_size.size() << '\n';
	for (auto v : a.layers_size) {
		out << v << ' ';
	}
	out << '\n';
	out << a.coefficient.size() << '\n';
	for (auto v : a.coefficient) {
		out << v << ' ';
	}
	return out;
}

inline Neural_coef::Neural_coef(const std::vector<size_t>& layers_size, const std::vector<double>& coefficient) :
	layers_size(layers_size),
	coefficient(coefficient) {}

class Neural_network {
	std::vector<std::shared_ptr<Layer> > layers;

public:
	Neural_network(const std::vector<Layer*>& layers);
	Neural_network(Neural_coef const& coef);

	std::vector<double> get(std::vector<double> const& data) const;
	std::vector<std::vector<std::vector<double> > > get_neurons_weight() const;
	Neural_coef get_coefficient() const;
};


inline Neural_network::Neural_network(const std::vector<Layer*>& _layers):
	layers(_layers.size()) {
	for (size_t i = 0; i < layers.size(); i++) {
		layers[i].reset(_layers[i]);
	}
}

inline Neural_network::Neural_network(Neural_coef const& coef) {
	My_stream stream(coef.coefficient);
	for (size_t i = 1; i < coef.layers_size.size(); i++) {
		layers.emplace_back(new Actiev_layer<active_function_B>(coef.layers_size[i - 1], coef.layers_size[i], stream));
	}
	assert(stream.end());
}

inline std::vector<double> Neural_network::get(std::vector<double> const& data) const {
	std::vector<double> result = data;
	for (size_t i = 0; i < layers.size(); i++) {
		result = layers[i]->get(result);
	}
	return result;
}

inline std::vector<std::vector<std::vector<double> > > Neural_network::get_neurons_weight() const {
	std::vector<std::vector<std::vector<double> > > result(layers.size());
	for (size_t i = 0; i < layers.size(); i++) {
		result[i] = layers[i]->get_neurons_weight();
	}
	return result;
}

inline Neural_coef Neural_network::get_coefficient() const {
	std::vector<double> result;
	for (size_t i = 0; i < layers.size(); i++) {
		auto temp = layers[i]->get_coefficient();
		for (auto v : temp) {
			result.push_back(v);
		}
	}
	std::vector<size_t> layers_size;

	layers_size.push_back(layers.front()->get_input_size());
	for (auto const& v : layers) {
		layers_size.push_back(v->get_output_size());
	}
	return Neural_coef(layers_size, result);
}
