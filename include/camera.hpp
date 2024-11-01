#pragma once

#include <limits>

#include "hittable.hpp"
#include "color.hpp"

class Camera {
public:
	Camera(Vec3 pos, int image_width, float aspect_ratio);

	void render(const Hittable& world, void* bits);

private:
	float aspect_ratio;
	int image_width;
	int image_height;
	Point3 pos;
	Point3 pixel00_loc;
	Vec3 pixel_dx;
	Vec3 pixel_dy;

	Color ray_color(const Ray& r, const Hittable& world);
};