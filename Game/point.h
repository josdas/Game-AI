#pragma once
#include <cmath>

struct Point {
	int x, y;

	Point(int x, int y);
	Point();

	int length() const;

	friend Point operator+(Point a, Point b);
	friend Point operator-(Point a, Point b);
	friend bool operator==(Point a, Point b);
};

inline Point::Point(int x, int y): x(x),
                                   y(y) {}

inline Point::Point(): Point(0, 0) {}

inline int Point::length() const {
	return std::abs(x) + std::abs(y);
}

inline Point operator-(Point a, Point b) {
	return Point(a.x - b.x, a.y - b.y);
}

inline bool operator==(Point a, Point b) {
	return a.x == b.x && a.y == b.y;
}

inline Point operator+(Point a, Point b) {
	return Point(a.x + b.x, a.y + b.y);
}