#pragma once

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

struct active_function_linear {
	static double active_function(double x) {
		return x > 0;
	}
};