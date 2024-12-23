#pragma once

#include "hittable.hpp"

class Sphere : public Hittable {
public:
	Sphere(const Point3& pos, float radius, shared_ptr<Material> mat) : pos(pos), radius(abs(radius)), mat(mat) {
        Vec3 rvec(radius, radius, radius);
        bbox = AABB(pos - rvec, pos + rvec);
    }

    bool hit(const Ray& r, Range range, Hit& hit) const override {
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
        if (!range.surrounds(root)) {
            root = (h + sqrtd) / a;
            if (!range.surrounds(root)) {
                return false;
            }
        }

        hit.t = root;
        hit.p = r.at(hit.t);
        hit.set_face_normal(r, (hit.p - pos) / radius);
        hit.mat = mat;

        return true;
    }

    AABB bounding_box() const override {
        return bbox;
    }

private:
    Point3 pos;
    shared_ptr<Material> mat;
    float radius;
    AABB bbox;
};