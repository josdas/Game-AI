#pragma once

struct Point {
	int x, y;

	Point(int x, int y)
		: x(x),
		  y(y) {}

	friend Point operator+(Point a, Point b) {
		return Point(a.x + b.x, a.y + b.y);
	}

	friend bool operator==(Point a, Point b) {
		return a.x == b.x && a.y == b.y;
	}
};
