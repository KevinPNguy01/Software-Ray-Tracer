#pragma once

#include "vec3.hpp"

class Ray {
private:
	Vec3 pos;
	Vec3 dir;

public:
	Ray() : pos(Point3()), dir(Vec3()) {}
	Ray(const Point3& pos, const Vec3& dir) : pos(pos), dir(dir) {}

	const Point3& position() const {
		return pos;
	}
	const Vec3& direction() const {
		return dir;
	}

	Point3 at(float f) const {
		return pos + dir * f;
	}
};