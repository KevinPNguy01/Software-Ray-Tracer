#pragma once

#include <limits>
#include <cstdlib>

extern const float infinity;
extern const float pi;

inline float degrees_to_radians(float degrees) {
    return degrees * pi / 180;
}

inline float random_float() {
    return std::rand() / (RAND_MAX + 1.0f);
}

inline float random_float(float min, float max) {
    return min + (max - min) * random_float();
}

inline int random_int(int min, int max) {
    return int(random_float(min, max + 1));
}