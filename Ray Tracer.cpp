#include <iostream>
#include <Windows.h>
#include "color.hpp"
#include "vec3.hpp"

int main() {
    int width = 256;
    int height = 256;

    HWND hwnd = GetConsoleWindow();
    HDC hdcDest = GetDC(GetConsoleWindow());

    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;  // Negative to make the image top-down
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;  // 32-bit color depth
    bmi.bmiHeader.biCompression = BI_RGB;

    void* bits;
    HBITMAP hBitmap = CreateDIBSection(hdcDest, &bmi, DIB_RGB_COLORS, &bits, NULL, 0);
    HDC hdcMem = CreateCompatibleDC(hdcDest);
    SelectObject(hdcMem, hBitmap);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const color c = color((float)x / width, (float)y / height, 0);
            ((DWORD*)bits)[y * width + x] = color_to_BGR(c);
        }
    }

    while (true) {
        BitBlt(GetDC(hwnd), 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);
        ReleaseDC(hwnd, hdcDest);
    }
}