#pragma once

#include "hittable.hpp"

class Sphere : Hittable {
private:
	Point3 pos;
	float radius;

public:
	Sphere(const Point3& pos, float radius) : pos(pos), radius(abs(radius)) {}

    bool hit(const Ray& r, float tmin, float tmax, Hit& hit) {
        Vec3 d = pos - r.position();
        float a = r.direction().length_squared();
        float h = dot(r.direction(), d);
        float c = d.length_squared() - radius * radius;
        float discriminant = h * h - a * c;

        if (discriminant < 0) {
            return false;
        }

        float sqrtd = sqrt(discriminant);
        float root = (h - sqrtd) / a;
        if (root <= tmin || tmax <= root) {
            root = (h + sqrtd) / a;
            if (root <= tmin || tmax <= root) {
                return false;
            }
        }

        hit.t = root;
        hit.p = r.at(hit.t);
        hit.set_face_normal(r, (hit.p - pos) / radius);

        return true;
    }
};