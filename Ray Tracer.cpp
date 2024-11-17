#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <Windows.h>
#include <cmath>
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <atomic>

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
#include "bvh.hpp"
#include "quad.hpp"
#include "diffuse_light.hpp"

using std::make_shared;
using std::shared_ptr;

constexpr int FPS = 60;
constexpr std::chrono::milliseconds frameDuration(1000 / FPS);

std::atomic<bool> input_flags[6];           // Flags indicating a key was held down during a frame.
std::atomic<bool> mouse_flag(false);
boolean inFocus = true;
boolean escapeKeyState = false;
POINT lastMousePos;
int dx, dy;

float aspect_ratio = 16.0 / 9;
int image_width = 800;
int image_height = max(1, int(image_width / aspect_ratio));

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void centerMouse(HWND hwnd) {
    POINT center = { image_width / 2, image_height / 2 };
    ClientToScreen(hwnd, &center);
    SetCursorPos(center.x, center.y);
}

void handleMouseMovement(Camera& cam, HWND hwnd) {
    POINT currentMousePos;
    GetCursorPos(&currentMousePos);

    // Calculate the difference in cursor position
    dx = currentMousePos.x - lastMousePos.x;
    dy = currentMousePos.y - lastMousePos.y;

    // Store the current position for the next frame
    lastMousePos = currentMousePos;

    if (dx != 0 && dy != 0) {
        cam.restart_render.store(true);
        mouse_flag.store(true);
    }
}

void handleInput(Camera& cam, HWND hwnd) {
    constexpr char keys[] = { 'W', 'A', 'S', 'D', VK_SPACE, VK_SHIFT };
    while (1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        // Unfocus window by pressing escape.
        if ((GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0) {
            if (!escapeKeyState) {
                escapeKeyState = true;
                inFocus = !inFocus;
                if (inFocus) centerMouse(hwnd);
            }
        }
        else {
            escapeKeyState = false;
        }
        if (!inFocus) continue;

        handleMouseMovement(cam, hwnd);
        // Raise flags for each of the keys that were held down during a frame.
        for (int i = 0; i < 6; ++i) {
            if (GetAsyncKeyState(keys[i]) & 0x8000) {
                input_flags[i].store(true);
                cam.restart_render.store(true);
            }
        }
    }
}

void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

int main() {
    const wchar_t CLASS_NAME[] = L"Realtime Raytracer";
    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    RECT rect = { 0, 0, image_width, image_height };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Realtime Raytracer",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
        NULL, NULL, wc.hInstance, NULL
    );
    ShowWindow(hwnd, SW_SHOW);
    ShowCursor(FALSE);

    HDC hdcDest = GetDC(hwnd);
    ShowConsoleCursor(false);

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

    shared_ptr<Lambertian> ground = make_shared<Lambertian>(Color(0.7, 0.7, 0.7));
    auto red = make_shared<Lambertian>(Color(.65, .05, .05));
    auto pink = make_shared<Lambertian>(Color(1, .78, .87));
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(Color(4,4,4));
    auto light_blue = make_shared<Lambertian>(Color(0.74, 0.88, 1));
    auto purple = make_shared<Lambertian>(Color(0.8, 0.71, 0.86));
    auto blue = make_shared<Lambertian>(Color(0.64, 0.82, 1));
    auto mirror = make_shared<Metal>(Color(1, 1, 1), 0);

    HittableList world;
    world.add(make_shared<Sphere>(Point3(0, -10000, 0), 10000, ground));
    world.add(make_shared<Quad>(Point3(-1.5, 3, -1.5), Vec3(6, 0, 0), Vec3(0, 0, 6), white));
    world.add(make_shared<Quad>(Point3(-1.5, 0, -1.5), Vec3(6, 0, 0), Vec3(0, 3, 0), pink));
    world.add(make_shared<Quad>(Point3(-1.5, 0, -1.5), Vec3(0, 0, 6), Vec3(0, 3, 0), pink));
    world.add(make_shared<Quad>(Point3(4.5, 0, 4.5), Vec3(-6, 0, 0), Vec3(0, 3, 0), pink));
    world.add(make_shared<Quad>(Point3(4.5, 0, 4.5), Vec3(0, 0, -6), Vec3(0, 3, 0), pink));
    world.add(make_shared<Quad>(Point3(-1.5, 0.5, -1), Vec3(0, 0, 2), Vec3(0, 2, 0), light));
    world.add(make_shared<Quad>(Point3(-0.75, 0.75, -1.5), Vec3(1.5, 0, 0), Vec3(0, 1.5, 0), mirror));

    world.add(make_shared<Quad>(Point3(-0.5, 0, -0.5), Vec3(0, 0, 1), Vec3(0, 1, 0), ground));
    world.add(make_shared<Quad>(Point3(-0.5, 0, -0.5), Vec3(1, 0, 0), Vec3(0, 1, 0), ground));
    world.add(make_shared<Quad>(Point3(0.5, 0, 0.5), Vec3(0, 0, -1), Vec3(0, 1, 0), ground));
    world.add(make_shared<Quad>(Point3(0.5, 0, 0.5), Vec3(-1, 0, 0), Vec3(0, 1, 0), ground));
    world.add(make_shared<Quad>(Point3(-0.5, 1, -0.5), Vec3(0, 0, 1), Vec3(1, 0, 0), ground));
    world.add(make_shared<Sphere>(Point3(0.25, 1.2, -0.25), 0.2, blue));
    world.add(make_shared<Sphere>(Point3(-0.25, 1.25, 0.25), 0.25, purple));

    world = HittableList(make_shared<BVH_Node>(world));

    Camera cam(Vec3(2.5, 1.5, 2.5), image_width, aspect_ratio);
    BitBlt(GetDC(hwnd), 0, 0, image_width, image_height, hdcMem, 0, 0, SRCCOPY);

    auto previous = std::chrono::high_resolution_clock::now();
    std::thread inputThread(handleInput, std::ref(cam), hwnd);
    bool running = true;
    while (running) {
        if (cam.samples_per_pixel == 65536 + 1) {
            continue;
        }
        while (ShowCursor(FALSE) >= 0);

        auto start = std::chrono::high_resolution_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(start - previous).count();

        // Render regions of the screen in separate threads.
        int n = cam.num_threads;
        int region_size = image_height / n + 1;
        std::vector<std::thread> threads;
        cam.restart_rendering();
        for (int i = 0; i < n; ++i) {
            threads.emplace_back(&Camera::render_region, &cam, std::ref(world), bits, i * region_size, min(image_height, (i + 1) * region_size));
        }
        while (!cam.done_rendering()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            MSG msg;
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT) {
                    running = false;
                }
                else {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
        }
        for (auto& thread : threads) {
            thread.join();
        }
        
        if (!cam.restart_render.load() || cam.samples_per_pixel == 1) {
            BitBlt(GetDC(hwnd), 0, 0, image_width, image_height, hdcMem, 0, 0, SRCCOPY);
        }
        cam.restart_render.load() ? cam.resetQuality() : cam.increaseQuality();
        cam.restart_render.store(false);

        // Iterate through the input flags and move camera accordingly.
        for (int i = 0; i < 6; ++i) {
            if (input_flags[i].load()) {
                cam.move(static_cast<Camera::direction>(i), 0.1);
            }
            input_flags[i].store(false);
        }

        // Translate mouse movement to camera rotation
        const float sensitivity = 0.25f; // Adjust sensitivity as needed
        cam.rotate_yaw(dx * sensitivity);  // Horizontal movement affects yaw
        cam.rotate_pitch(dy * sensitivity); // Vertical movement affects pitch
        cam.autoFocus(world);
        cam.initialize();
        if (inFocus) centerMouse(hwnd);
        mouse_flag.store(false);

        previous = start;
        std::this_thread::sleep_for(frameDuration);
    }
    ReleaseDC(hwnd, hdcDest);
}