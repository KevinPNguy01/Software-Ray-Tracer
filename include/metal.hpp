#pragma once

#include "lambertian.hpp"

class Metal : public Material {
public:
	Metal(const Color& albedo) : Material(albedo) {}

	bool scatter(const Ray& r, const Hit& hit, Color& attenuation, Ray& scattered) const override {
		Vec3 dir = reflect(r.direction(), hit.normal);
		scattered = Ray(hit.p, dir);
		attenuation = albedo;
		return true;
	}
};