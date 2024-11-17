#pragma once

#include "material.hpp"

class DiffuseLight : public Material {
public:
	DiffuseLight(const Color& color) : color(color) {}

	Color emitted(Point3& p) const override {
		return color;
	}

	bool scatter(const Ray& r, const Hit& hit, Color& attenuation, Ray& scattered) const {
		return false;
	}

private:
	Color color;
};