#pragma once
#include <cstdlib>

inline double get_rand_double(double s) {
	return 2.0 * (rand() - RAND_MAX / 2) / RAND_MAX * s;
}

inline int get_rand_int(int x) {
	return rand() % x;
}
