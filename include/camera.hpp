#pragma once

#include <limits>

#include "hittable.hpp"
#include "color.hpp"
#include "util.hpp"

class Camera {
public:
	Camera(Vec3 pos, int image_width, float aspect_ratio);

	void render(const Hittable& world, void* bits);

private:
	float aspect_ratio = 1;
	int image_width = 400;
	int image_height;
	int samples_per_pixel = 10;
	float pixel_samples_scale;
	Point3 pos;
	Point3 pixel00_loc;
	Vec3 pixel_dx;
	Vec3 pixel_dy;

	Ray get_ray(int x, int y);
	Color ray_color(const Ray& r, const Hittable& world);
};