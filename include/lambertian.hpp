#pragma once

#include "material.hpp"

class Lambertian : public Material {
public:
	Lambertian(const Color& albedo) : albedo(albedo) {}

	bool scatter(const Ray& r, const Hit& hit, Color& attenuation, Ray& scattered) const override {
		Vec3 dir = hit.normal + random_unit_vector();
		if (dir.length_squared() < 1e-16) {
			dir = hit.normal;
		}

		scattered = Ray(hit.p, dir);
		attenuation = albedo;
		return true;
	}

protected:
	Color albedo;
};