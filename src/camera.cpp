#include "camera.hpp"

Camera::Camera(Vec3 look_from, int image_width, float aspect_ratio) : look_from(look_from), image_width(image_width), aspect_ratio(aspect_ratio), restart_render(false) {
    initialize();
}

Ray Camera::get_ray(int x, int y) {
    float offset_x = random_float() - 0.5;
    float offset_y = random_float() - 0.5;
    Vec3 dir = pixel00_loc + (offset_x + x) * pixel_dx + (offset_y + y) * pixel_dy - look_from;
    return Ray(look_from, dir);
}

Color Camera::ray_color(const Ray& r, int depth, const Hittable& world) {
    if (depth <= 0) {
        return Color();
    }

    Hit hit;
    if (world.hit(r, Range(0.001, std::numeric_limits<float>::infinity()), hit)) {
        Ray scattered;
        Color attenuation;
        if (hit.mat->scatter(r, hit, attenuation, scattered)) {
            return attenuation * ray_color(scattered, depth-1, world);
        }
        return Color();
    }
    
    Vec3 unit = unit_vector(r.direction());
    float a = 0.5 * (unit.y() + 1);
    return (1.0 - a) * Color(1, 1, 1) + a * Color(0.5, 0.7, 1);
}

void Camera::move(direction dir, float amount) {
    Vec3 delta;
    switch (dir) {
    case UP:
        delta = Vec3(0, 1, 0);
        break;
    case DOWN:
        delta = Vec3(0, -1, 0);
        break;
    case LEFT:
        delta = -u;
        break;
    case RIGHT:
        delta = u;
        break;
    case FORWARD:
        delta = unit_vector(-w * Vec3(1, 0, 1));
        break;
    case BACKWARD:
        delta = unit_vector(w * Vec3(1, 0, 1));
        break;
    }

    look_from += delta * amount;
    initialize();
}

void Camera::initialize() {
    image_height = max(1, int(image_width / aspect_ratio));
    pixel_samples_scale = 1.0 / samples_per_pixel;

    float focal_length = 1;
    float theta = degrees_to_radians(vfov);
    float h = tan(theta / 2);
    float viewport_height = 2 * h * focal_length;
    float viewport_width = viewport_height * ((float)image_width / image_height);

    Vec3 forward(
        cos(degrees_to_radians(yaw)) * cos(degrees_to_radians(pitch)),
        sin(degrees_to_radians(pitch)),
        sin(degrees_to_radians(yaw)) * cos(degrees_to_radians(pitch))
    );

    w = unit_vector(forward);
    u = cross(Vec3(0, 1, 0), w);
    v = cross(w, u);

    Vec3 viewport_x = viewport_width * u;
    Vec3 viewport_y = -viewport_height * v;
    pixel_dx = viewport_x / image_width;
    pixel_dy = viewport_y / image_height;

    Vec3 viewport_upper_left = look_from - viewport_x / 2 - viewport_y / 2 - focal_length * w;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_dx + pixel_dy);
}

void Camera::increaseQuality() {
    samples_per_pixel = min(2048, samples_per_pixel * 2);
    max_depth = min(24, max_depth + 2);
    initialize();
}

void Camera::resetQuality() {
    samples_per_pixel = 1;
    max_depth = 2;
}

void Camera::render_region(const Hittable& world, void* bits, int start_y, int end_y) {
    for (int y = start_y; y < end_y; ++y) {
        for (int x = 0; x < image_width; ++x) {
            if (restart_render.load() && samples_per_pixel > 1) {
                return;
            }
            Vec3 pixel_center = pixel00_loc + x * pixel_dx + y * pixel_dy;
            Vec3 ray_dir = pixel_center - look_from;
            Color c;
            for (int i = 0; i < samples_per_pixel; ++i) {
                Ray r = get_ray(x, y);
                c += ray_color(r, max_depth, world);
            }
            ((DWORD*)bits)[y * image_width + x] = color_to_BGR(correct_gamma(c * pixel_samples_scale));
        }
    }
}

void Camera::rotate_yaw(float angle) {
    yaw += angle;
    initialize();
}

void Camera::rotate_pitch(float angle) {
    pitch += angle;
    initialize();
}