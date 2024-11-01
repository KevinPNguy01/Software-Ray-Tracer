#pragma once

#include "vec3.hpp"

class Ray {
private:
	Vec3 pos;
	Vec3 dir;

public:
	Ray(const Point3& pos, const Vec3& dir) : pos(pos), dir(dir) {}

	const Point3& pos() {
		return pos;
	}
	const Vec3& dir() {
		return dir;
	}

	Point3 at(float f) {
		return pos + dir * f;
	}
};