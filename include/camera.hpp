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
	int samples_per_pixel = 5;
	int max_depth = 5;
	float pixel_samples_scale;
	float vfov = 70;

	Point3 look_from;
	Point3 look_at = Point3(0, 0, -1);
	Vec3 vup = Vec3(0, 1, 0);

	Point3 pixel00_loc;
	Vec3 pixel_dx;
	Vec3 pixel_dy;

	// Camera basis vectors.
	Vec3 u, v, w;

	Ray get_ray(int x, int y);
	Color ray_color(const Ray& r, int depth, const Hittable& world);
};