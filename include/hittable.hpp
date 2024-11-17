#pragma once

#include <memory>

#include "ray.hpp"
#include "aabb.hpp"
#include "range.hpp"
#include "material.hpp"

class Hit {
public:
	Point3 p;
	Vec3 normal, u, v;
	std::shared_ptr<Material> mat;
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

	virtual AABB bounding_box() const = 0;
};