#include <Windows.h>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

#include "color.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "range.hpp"

using std::make_shared;
using std::shared_ptr;

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

Color ray_color(Ray r, HittableList& objects) {
    Hit hit;
    if (objects.hit(r, Range(0, infinity), hit)) {
        return 0.5 * (hit.normal + Color(1, 1, 1));
    }
    
    Vec3 unit = unit_vector(r.direction());
    float a = 0.5 * (unit.y() + 1);
    return (1.0 - a) * Color(1, 1, 1) + a * Color(0.5, 0.7, 1);
}

int main() {
    float aspect_ratio = 16.0 / 9;
    int image_width = 800;
    int image_height = max(1, int(image_width / aspect_ratio));

    // Camera
    float focal_length = 1;
    float viewport_height = 2;
    float viewport_width = viewport_height * ((float)image_width / image_height);
    Vec3 camera_center = Point3();

    Vec3 viewport_x = Vec3(viewport_width, 0, 0);
    Vec3 viewport_y = Vec3(0, -viewport_height, 0);
    Vec3 pixel_dx = viewport_x / image_width;
    Vec3 pixel_dy = viewport_y / image_height;

    Vec3 viewport_upper_left = camera_center - viewport_x / 2 - viewport_y / 2 - Vec3(0, 0, focal_length);
    Vec3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_dx + pixel_dy);

    HWND hwnd = GetConsoleWindow();
    HDC hdcDest = GetDC(GetConsoleWindow());

    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
    bmi.bmiHeader.biWidth = image_width;
    bmi.bmiHeader.biHeight = -image_height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    void* bits;
    HBITMAP hBitmap = CreateDIBSection(hdcDest, &bmi, DIB_RGB_COLORS, &bits, NULL, 0);
    HDC hdcMem = CreateCompatibleDC(hdcDest);
    SelectObject(hdcMem, hBitmap);

    HittableList world;
    world.add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.add(make_shared<Sphere>(Point3(0, -10000.5, -1), 10000));

    for (int y = 0; y < image_height; ++y) {
        for (int x = 0; x < image_width; ++x) {
            Vec3 pixel_center = pixel00_loc + x * pixel_dx + y * pixel_dy;
            Vec3 ray_dir = pixel_center - camera_center;
            Ray r(camera_center, ray_dir);
            Color c = ray_color(r, world);
            ((DWORD*)bits)[y * image_width + x] = color_to_BGR(c);
        }
    }
    BitBlt(GetDC(hwnd), 0, 0, image_width, image_height, hdcMem, 0, 0, SRCCOPY);

    while (true) {
    }
    ReleaseDC(hwnd, hdcDest);
}