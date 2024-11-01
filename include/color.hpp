#pragma once

#include "vec3.hpp"
#include <Windows.h>

using Color = Vec3;

COLORREF RGBtoBGR(COLORREF color) {
    BYTE red = GetRValue(color);
    BYTE green = GetGValue(color);
    BYTE blue = GetBValue(color);

    return RGB(blue, green, red);
}

inline COLORREF color_to_BGR(const Color& c) {
    return RGBtoBGR(RGB(int(255 * c.buf[0]), int(255 * c.buf[1]), int(255 * c.buf[2])));
}