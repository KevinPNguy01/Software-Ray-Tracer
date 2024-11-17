#pragma once

#include <limits>
#include <atomic>
#include <mutex>

#include "hittable.hpp"
#include "color.hpp"
#include "util.hpp"

class Camera {
public:
	int samples_per_pixel = 1;
	int num_threads = 8;

	std::atomic<bool> restart_render;

	static enum direction {FORWARD, LEFT, BACKWARD, RIGHT, UP, DOWN};

	Camera(Vec3 look_from, int image_width, float aspect_ratio);

	void render_region(const Hittable& world, void* bits, int start_y, int end_y);

	void initialize();
	void autoFocus(const Hittable& world);

	void move(direction dir, float amount);
	void rotate_yaw(float angle);
	void rotate_pitch(float angle);

	void increaseQuality();
	void resetQuality();

	void restart_rendering();
	bool done_rendering();

private:
	float aspect_ratio = 1;
	int image_width = 400;
	int image_height;
	int max_depth = 2;
	float pixel_samples_scale;
	float vfov = 40;
	float defocus_angle = 2;
	float focus_dist = 1;

	float yaw = 0;
	float pitch = 0;

	Point3 look_from;
	Vec3 vup = Vec3(0, 1, 0);

	Point3 pixel00_loc;
	Vec3 pixel_dx;
	Vec3 pixel_dy;

	// Camera basis vectors.
	Vec3 u, v, w;

	Vec3 defocus_disk_u;
	Vec3 defocus_disk_v;

	Ray get_ray(int x, int y);
	Color ray_color(const Ray& r, int depth, const Hittable& world);

	std::mutex lock;
	int workers_done = 0;
};