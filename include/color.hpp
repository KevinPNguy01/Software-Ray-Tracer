#pragma once

#include <Windows.h>

#include "vec3.hpp"
#include "range.hpp"

using Color = Vec3;

inline COLORREF RGBtoBGR(COLORREF color) {
    return RGB(GetBValue(color), GetGValue(color), GetRValue(color));
}

inline COLORREF color_to_BGR(const Color& c) {
    return RGBtoBGR(RGB(int(255 * c.buf[0]), int(255 * c.buf[1]), int(255 * c.buf[2])));
}

inline float linear_to_gamma(const float val) {
    return val > 0 ? sqrt(val) : 0;
}

inline Color correct_gamma(const Color& c) {
    return Color(linear_to_gamma(c.x()), linear_to_gamma(c.y()), linear_to_gamma(c.z()));
}