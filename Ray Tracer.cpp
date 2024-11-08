#include <Windows.h>
#include <cmath>
#include <iostream>
#include <memory>

#include "color.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "range.hpp"
#include "camera.hpp"
#include "util.hpp"
#include "metal.hpp"
#include "lambertian.hpp"

using std::make_shared;
using std::shared_ptr;

int main() {
    float aspect_ratio = 16.0 / 9;
    int image_width = 800;
    int image_height = max(1, int(image_width / aspect_ratio));

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

    shared_ptr<Lambertian> ground = make_shared<Lambertian>(Color(0.5,0.5,0.5));
    shared_ptr<Lambertian> red = make_shared<Lambertian>(Color(1,0,0));
    shared_ptr<Lambertian> blue = make_shared<Lambertian>(Color(0,0,1));
    shared_ptr<Metal> metal = make_shared<Metal>(Color(0.5, 0.5, 0.5), 0);

    HittableList world;
    world.add(make_shared<Sphere>(Point3(0, 0, -1.5), 0.5, metal));
    world.add(make_shared<Sphere>(Point3(1, 0, -1), 0.5, red));
    world.add(make_shared<Sphere>(Point3(-1, 0, -1), 0.5, blue));
    world.add(make_shared<Sphere>(Point3(0, -100.5, -1), 100, ground));

    Camera cam(Vec3(-2, 2, 1), image_width, aspect_ratio);
    cam.render(world, bits);

    BitBlt(GetDC(hwnd), 0, 0, image_width, image_height, hdcMem, 0, 0, SRCCOPY);

    while (true) {
    }
    ReleaseDC(hwnd, hdcDest);
}