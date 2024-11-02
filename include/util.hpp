#pragma once

#include <limits>
#include <cstdlib>

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385;

inline float degrees_to_radians(float degrees) {
    return degrees * pi / 180.0;
}

inline float random_float() {
    return std::rand() / (RAND_MAX + 1.0);
}

inline double random_float(float min, float max) {
    return min + (max - min) * random_float();
}