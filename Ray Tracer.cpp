#include <iostream>
#include <Windows.h>

COLORREF RGBtoBGR(COLORREF color) {
    BYTE red = GetRValue(color);
    BYTE green = GetGValue(color);
    BYTE blue = GetBValue(color);

    return RGB(blue, green, red);
}

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
            double r = (double)x / width;
            double g = (double)y / height;
            double b = 0;

            int ir = int(255 * r);
            int ig = int(255 * g);
            int ib = int(255 * b);

            ((DWORD*)bits)[y * width + x] = RGBtoBGR(RGB(ir, ig, ib));
        }
    }

    while (true) {
        BitBlt(GetDC(hwnd), 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);
        ReleaseDC(hwnd, hdcDest);
    }
}