#pragma once

#include "hittable.hpp"

class Quad : public Hittable {
public:
	Quad(const Point3& Q, const Vec3& u, const Vec3& v, std::shared_ptr<Material> mat) : Q(Q), u(u), v(v), mat(mat) {
		Vec3 n = cross(u, v);
		normal = unit_vector(n);
		D = dot(normal, Q);
		w = n / dot(n, n);
		set_bounding_box();
	}

	 void set_bounding_box() {
		bbox = AABB(AABB(Q, Q + u + v), AABB(Q + u, Q + v));
	}

	AABB bounding_box() const override {
		return bbox;
	}

	bool hit(const Ray& r, Range t_range, Hit& hit) const override {
		float denom = dot(normal, r.direction());

		if (std::fabs(denom) < 1e-8) {
			return false;
		}

		float t = (D - dot(normal, r.position())) / denom;
		if (!t_range.contains(t)) {
			return false;
		}

		Point3 intersection = r.at(t);
		Vec3 p = intersection - Q;
		float a = dot(w, cross(p, v));
		float b = dot(w, cross(u, p));
		Range zero_one(0, 1);
		if (!zero_one.contains(a) || !zero_one.contains(b)) {
			return false;
		}

		hit.t = t;
		hit.p = intersection;
		hit.u = u;
		hit.v = v;
		hit.mat = mat;
		hit.set_face_normal(r, normal);
		return true;
	}

private:
	Point3 Q;
	Vec3 u, v;
	std::shared_ptr<Material> mat;
	AABB bbox;
	Vec3 normal;
	Vec3 w;
	float D;
};