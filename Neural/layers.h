#pragma once
#include "neuron.h"

template<class T>
class Layer {
	std::vector<std::vector<double> > neurons;

public:
	explicit Layer(const std::vector<std::vector<double> >& neurons);
	explicit Layer(size_t s, size_t past);

	std::vector<double> get(std::vector<double> const& data) const;
	std::vector<std::vector<double> > const& get_neurons_weight() const;
	std::vector<double> get_coefficient() const;

	void set(size_t a, size_t b, double x);
	void resize(size_t n);
};

template <class T>
Layer<T>::Layer(const std::vector<std::vector<double> >& neurons): neurons(neurons) {}

template <class T>
Layer<T>::Layer(size_t s, size_t past) {
	s++ , past++; //todo
	neurons.resize(s);
	for (size_t i = 0; i < s; i++) {
		neurons[i].resize(past);
		for (size_t j = 0; j < past; j++) {
			neurons[i][j] = get_rand_double(0.5);
		}
	}
}

template <class T>
std::vector<double> Layer<T>::get(std::vector<double> const& data) const {
	std::vector<double> result(neurons.size() - 1);
	for (size_t i = 0; i + 1 < neurons.size(); i++) {
		double sum = neurons[i].back();
		//#pragma omp parallel for reduction(+:sum)
		for (int j = 0; j < static_cast<int>(data.size()); j++) {
			sum += data[j] * neurons[i][j];
		}
		//#pragma omp barrier
		result[i] = T::active_function(sum);
	}
	return result;
}

template <class T>
std::vector<std::vector<double> > const& Layer<T>::get_neurons_weight() const {
	return neurons;
}

template <class T>
std::vector<double> Layer<T>::get_coefficient() const {
	std::vector<double> result;
	for (size_t i = 0; i < neurons.size(); i++) {
		for (auto v : neurons[i]) {
			result.push_back(v);
		}
	}
	return result;
}

template <class T>
void Layer<T>::set(size_t a, size_t b, double x) {
	neurons[a][b] = x;
}

template <class T>
void Layer<T>::resize(size_t n) {
	for (auto& v : neurons) {
		v.resize(n);
	}
}
