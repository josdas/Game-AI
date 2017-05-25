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