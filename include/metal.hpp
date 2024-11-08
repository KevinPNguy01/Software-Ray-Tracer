#pragma once

#include "lambertian.hpp"

class Metal : public Lambertian {
public:
	Metal(const Color& albedo) : Lambertian(albedo) {}

	bool scatter(const Ray& r, const Hit& hit, Color& attenuation, Ray& scattered) const override {
		Vec3 dir = reflect(r.direction(), hit.normal);
		scattered = Ray(hit.p, dir);
		attenuation = albedo;
		return true;
	}
};