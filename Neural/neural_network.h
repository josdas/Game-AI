#pragma once
#include <vector>
#include "layers.h"

struct Neural_coef {
	std::vector<size_t> layers_size;
	std::vector<double> coefficient; 
	
	Neural_coef(const std::vector<size_t>& layers_size, const std::vector<double>& coefficient)
		: layers_size(layers_size),
		coefficient(coefficient) {}
};

struct Neural_network {
	std::vector<Layer> layers;
	std::vector<size_t> layers_size;

	Neural_network(const std::vector<Layer>& layers, const std::vector<size_t>& layers_size)
		: layers(layers),
		layers_size(layers_size) {}

	Neural_network(std::vector<size_t> const& S) :
		layers_size(S) {
		for (size_t i = 1; i < S.size(); i++) {
			layers.emplace_back(S[i], S[i - 1]);
		}
	}

	Neural_network(Neural_coef const& coef) :
		Neural_network(coef.layers_size) {
		size_t ind = 0;
		for (size_t i = 1; i < coef.layers_size.size(); i++) {
			for(size_t j = 0; j < coef.layers_size[i]; j++) {
				for(size_t k = 0; k < coef.layers_size[i - 1]; k++, ind++) {
					layers[i - 1].set(j, k, coef.coefficient[ind]);
				}
			}
		}
	}

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

	Neural_coef get_coefficient() const {
		std::vector<double> result;
		for (size_t i = 0; i < layers.size(); i++) {
			auto temp = layers[i].get_coefficient();
			for (auto v : temp) {
				result.push_back(v);
			}
		}
		return Neural_coef(layers_size, result);
	}
};
