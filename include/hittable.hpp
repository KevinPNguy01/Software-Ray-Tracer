#pragma once

#include "ray.hpp"
#include "range.hpp"

class Hit {
public:
	Point3 p;
	Vec3 normal;
	float t;
	bool front_face;

	/**
	 *	@param outward_normal is assumed to be unit length.
	 */
	void set_face_normal(const Ray& r, const Vec3& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class Hittable {
public:
	virtual bool hit(const Ray& r, Range range, Hit& hit) const = 0;
};