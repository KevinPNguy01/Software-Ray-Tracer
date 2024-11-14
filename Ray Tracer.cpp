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
#include "dielectric.hpp"

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
    shared_ptr<Dielectric> purple = make_shared<Dielectric>(Color(1,1,1), 1/1.5);
    shared_ptr<Lambertian> pink = make_shared<Lambertian>(Color(1, 0.7843137254901961, 0.8666666666666667));
    shared_ptr<Lambertian> dark_pink = make_shared<Lambertian>(Color(1, 0.6862745098039216, 0.8));
    shared_ptr<Metal> metal = make_shared<Metal>(Color(0.9, 0.9, 0.9), 0.01);

    HittableList world;
    world.add(make_shared<Sphere>(Point3(0, 0, -1.5), 0.5, metal));
    world.add(make_shared<Sphere>(Point3(1, 0, -1), 0.5, pink));
    world.add(make_shared<Sphere>(Point3(-1, 0, -1), 0.5, purple));
    world.add(make_shared<Sphere>(Point3(-0.25, -0.4, -0.75), 0.1, dark_pink));
    world.add(make_shared<Sphere>(Point3(0, -100.5, -1), 100, ground));

    Camera cam(Vec3(0, 1, 1), image_width, aspect_ratio);
    BitBlt(GetDC(hwnd), 0, 0, image_width, image_height, hdcMem, 0, 0, SRCCOPY);

    auto previous = std::chrono::high_resolution_clock::now();
    std::thread inputThread(handleInput, std::ref(cam), hwnd);
    bool running = true;
    while (running) {
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
        while (ShowCursor(FALSE) >= 0);

        auto start = std::chrono::high_resolution_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(start - previous).count();

        // Render regions of the screen in separate threads.
        const int num_threads = 16;
        int region_size = image_height / 16 + 1;
        std::vector<std::thread> threads;
        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back(&Camera::render_region, &cam, std::ref(world), bits, i * region_size, min(image_height, (i + 1) * region_size));
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
        cam.initialize();
        if (inFocus) centerMouse(hwnd);
        mouse_flag.store(false);

        previous = start;
        std::this_thread::sleep_for(frameDuration);
    }
    ReleaseDC(hwnd, hdcDest);
}