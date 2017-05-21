#pragma once
#include "point.h"

struct Field {
	int h, w;

	Field(int h, int w)
		: h(h),
		  w(w) {}

	bool inside(Point p) const {
		return 0 <= p.x && p.x < h && 0 <= p.y && p.y < w;
	}
};
