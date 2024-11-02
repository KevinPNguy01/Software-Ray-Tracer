#include "camera.hpp"

Camera::Camera(Vec3 pos, int image_width, float aspect_ratio) : pos(pos), image_width(image_width), aspect_ratio(aspect_ratio) {
    image_height = max(1, int(image_width / aspect_ratio));
    pixel_samples_scale = 1.0 / samples_per_pixel;

    float focal_length = 1;
    float viewport_height = 2;
    float viewport_width = viewport_height * ((float)image_width / image_height);

    Vec3 viewport_x = Vec3(viewport_width, 0, 0);
    Vec3 viewport_y = Vec3(0, -viewport_height, 0);
    pixel_dx = viewport_x / image_width;
    pixel_dy = viewport_y / image_height;

    Vec3 viewport_upper_left = pos - viewport_x / 2 - viewport_y / 2 - Vec3(0, 0, focal_length);
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_dx + pixel_dy);
}

void Camera::render(const Hittable& world, void* bits) {
    for (int y = 0; y < image_height; ++y) {
        for (int x = 0; x < image_width; ++x) {
            Vec3 pixel_center = pixel00_loc + x * pixel_dx + y * pixel_dy;
            Vec3 ray_dir = pixel_center - pos;
            Color c;
            for (int i = 0; i < samples_per_pixel; ++i) {
                Ray r = get_ray(x, y);
                c += ray_color(r, world);
            }
            ((DWORD*)bits)[y * image_width + x] = color_to_BGR(c * pixel_samples_scale);
        }
    }
}

Ray Camera::get_ray(int x, int y) {
    float offset_x = random_float() - 0.5;
    float offset_y = random_float() - 0.5;
    Vec3 dir = pixel00_loc + (offset_x + x) * pixel_dx + (offset_y + y) * pixel_dy - pos;
    return Ray(pos, dir);
}

Color Camera::ray_color(const Ray& r, const Hittable& world) {
    Hit hit;
    if (world.hit(r, Range(0, std::numeric_limits<float>::infinity()), hit)) {
        return 0.5 * (hit.normal + Color(1, 1, 1));
    }
    
    Vec3 unit = unit_vector(r.direction());
    float a = 0.5 * (unit.y() + 1);
    return (1.0 - a) * Color(1, 1, 1) + a * Color(0.5, 0.7, 1);
}