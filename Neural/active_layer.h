#pragma once
#include "neuron.h"
#include <vector>
#include "layers.h"
#include <cassert>

template<class T>
class Actiev_layer: public Layer {
protected:
	std::vector<double> active(std::vector<double> data) const;
	std::vector<double> get_sum(std::vector<double> const& data) const;
public:
	explicit Actiev_layer(const std::vector<std::vector<double> >& neurons);
	Actiev_layer(size_t in, size_t out);
	Actiev_layer(size_t in, size_t out, My_stream& stream);
	~Actiev_layer() override = default;

	virtual std::vector<double> get(std::vector<double> const& data) const override;
	int get_type() override;
};

template <class T>
std::vector<double> Actiev_layer<T>::active(std::vector<double> data) const {
	for(auto &v : data) {
		v = T::active_function(v);
	}
	return data;
}

template <class T>
std::vector<double> Actiev_layer<T>::get_sum(std::vector<double> const& data) const {
	assert(input_size == data.size());
	std::vector<double> result(output_size);
	for (size_t i = 0; i < output_size; i++) {
		double sum = 0;
		//#pragma omp parallel for reduction(+:sum)
		for (int j = 0; j < input_size; j++) {
			sum += data[j] * neurons[i][j];
		}
		//#pragma omp barrier
		result[i] = sum;
	}
	return result;
}

template <class T>
Actiev_layer<T>::Actiev_layer(const std::vector<std::vector<double> >& neurons): Layer(neurons) {}

template <class T>
Actiev_layer<T>::Actiev_layer(size_t in, size_t out): Layer(in, out) {}

template <class T>
Actiev_layer<T>::Actiev_layer(size_t in, size_t out, My_stream& stream):
	Layer(in, out, stream) {}

template <class T>
std::vector<double> Actiev_layer<T>::get(std::vector<double> const& data) const {
	return active(get_sum(data));
}

template <class T>
int Actiev_layer<T>::get_type() {
	return T::type_number;
}
