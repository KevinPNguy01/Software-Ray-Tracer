#pragma once

#include "vec3.hpp"
#include <Windows.h>

using Color = Vec3;

inline COLORREF RGBtoBGR(COLORREF color) {
    return RGB(GetBValue(color), GetGValue(color), GetRValue(color));
}

inline COLORREF color_to_BGR(const Color& c) {
    return RGBtoBGR(RGB(int(255 * c.buf[0]), int(255 * c.buf[1]), int(255 * c.buf[2])));
}