#pragma once

#include "lambertian.hpp"

class Metal : public Material {
public:
	Metal(const Color& albedo, float fuzz) : Material(albedo), fuzz(min(fuzz, 1)) {}
	Metal(const Color& albedo) : Metal(albedo, 0) {}

	bool scatter(const Ray& r, const Hit& hit, Color& attenuation, Ray& scattered) const override {
		Vec3 dir = reflect(r.direction(), hit.normal);
		dir = unit_vector(dir) + (fuzz * random_unit_vector());
		scattered = Ray(hit.p, dir);
		attenuation = albedo;
		return dot(dir, hit.normal) > 0;
	}

private:
	float fuzz;
};