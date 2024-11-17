#pragma once

#include "range.hpp"
#include "vec3.hpp"
#include "ray.hpp"

class AABB {
public:
	Range x, y, z;

	AABB() {}

	AABB(const Range& x, const Range& y, const Range& z) : x(x), y(y), z(z) {
		pad_to_minimum();
	}

	AABB(const Point3& a, const Point3& b) {
		x = (a[0] <= b[0]) ? Range(a[0], b[0]) : Range(b[0], a[0]);
		y = (a[1] <= b[1]) ? Range(a[1], b[1]) : Range(b[1], a[1]);
		z = (a[2] <= b[2]) ? Range(a[2], b[2]) : Range(b[2], a[2]);
		pad_to_minimum();
	}

	AABB(const AABB& bbox0, const AABB& bbox1) {
		x = Range(bbox0.x, bbox1.x);
		y = Range(bbox0.y, bbox1.y);
		z = Range(bbox0.z, bbox1.z);
	}

	const Range& axis_range(int n) const {
		if (n == 0) return x;
		if (n == 1) return y;
		return z;
	}

	bool hit(const Ray& r, Range t_range) const {
		const Point3& pos = r.position();
		const Vec3& dir = r.direction();

		for (int i = 0; i < 3; ++i) {
			const Range& axis = axis_range(i);
			const float adinv = 1.0 / dir[i];

			float t0 = (axis.start - pos[i]) * adinv;
			float t1 = (axis.end - pos[i]) * adinv;

			if (t0 < t1) {
				if (t0 > t_range.start) t_range.start = t0;
				if (t1 < t_range.end) t_range.end = t1;
			}
			else {
				if (t1 > t_range.start) t_range.start = t1;
				if (t0 < t_range.end) t_range.end = t0;
			}

			if (t_range.end <= t_range.start)
				return false;
		}
		return true;
	}

	int longest_axis() const {
		if (x.size() > y.size()) {
			return x.size() > z.size() ? 0 : 2;
		}
		else {
			return y.size() > z.size() ? 1 : 2;
		}
	}

	void pad_to_minimum() {
		float delta = 0.0001;
		if (x.size() < delta) x.expand(delta);
		if (y.size() < delta) y.expand(delta);
		if (z.size() < delta) z.expand(delta);
	}

	static const AABB empty, universe;
};