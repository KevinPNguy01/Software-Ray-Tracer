#pragma once

#include "ray.hpp"
#include "hittable.hpp"
#include "color.hpp"

class Hit;

class Material {
public:
	Material(const Color& albedo) : albedo(albedo) {}

	virtual bool scatter(const Ray& r, const Hit& hit, Color& attenuation, Ray& scattered) const = 0;

protected:
	Color albedo;
};