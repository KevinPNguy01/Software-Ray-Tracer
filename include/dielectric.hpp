#include <cmath>

#include "material.hpp"

class Dielectric : public Material {
public:
	Dielectric(Color albedo, float refraction_index) : Material(albedo), refraction_index(refraction_index) {}

	bool scatter(const Ray& r, const Hit& hit, Color& attenuation, Ray& scattered) const override {
		float refraction_ratio = hit.front_face ? 1.0f / refraction_index : refraction_index;
		Vec3 unit_ray = unit_vector(r.direction());
		float cos_theta = std::fmin(dot(-unit_ray, hit.normal), 1);
		float sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);
		Vec3 dir;

		if (refraction_index * sin_theta > 1) {
			dir = reflect(unit_ray, hit.normal);
		}
		else {
			dir = refract(unit_ray, hit.normal, refraction_ratio);
		}

		scattered = Ray(hit.p, dir);
		attenuation = albedo;
		return true;
	}

private:
	float refraction_index;

	// Unused for now.
	static float reflectance(float cos, float refraction_index) {
		float r0 = (1 - refraction_index) / (1 + refraction_index);
		r0 *= r0;
		return r0 + (1 - r0) * std::pow((1 - cos), 5);
	}
};