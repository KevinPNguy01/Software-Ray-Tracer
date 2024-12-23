#pragma once

#include <cmath>
#include <iostream>

#include "util.hpp"

class Vec3 {
public:
	float buf[3];

	Vec3(float x, float y, float z) : buf{ x, y, z } {}
	Vec3() : buf{ 0, 0, 0 } {}

	float x() const {
		return buf[0];
	}
	float y() const {
		return buf[1];
	}
	float z() const {
		return buf[2];
	}


	Vec3 operator-() const {
		return Vec3(-buf[0], -buf[1], -buf[2]);
	}
	float& operator[](int i) {
		return buf[i];
	}
	float operator[](int i) const {
		return buf[i];
	}

	Vec3& operator+=(const Vec3& v) {
		buf[0] += v.buf[0];
		buf[1] += v.buf[1];
		buf[2] += v.buf[2];
		return *this;
	}
	Vec3& operator*=(float f) {
		buf[0] *= f;
		buf[1] *= f;
		buf[2] *= f;
		return *this;
	}
	Vec3& operator/=(float f) {
		buf[0] /= f;
		buf[1] /= f;
		buf[2] /= f;
		return *this;
	}

	float length_squared() const {
		return buf[0] * buf[0] + buf[1] * buf[1] + buf[2] * buf[2];
	}
	float length() const {
		return std::sqrt(length_squared());
	}

	static Vec3 random() {
		return Vec3(random_float(), random_float(), random_float());
	}

	static Vec3 random(float min, float max) {
		return Vec3(random_float(min, max), random_float(min, max), random_float(min, max));
	}
};

inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
	return out << v.buf[0] << " " << v.buf[1] << " " << v.buf[1];
}

inline Vec3 operator+(const Vec3& v1, const Vec3& v2) {
	return Vec3(v1.buf[0] + v2.buf[0], v1.buf[1] + v2.buf[1], v1.buf[2] + v2.buf[2]);
}

inline Vec3 operator-(const Vec3& v1, const Vec3& v2) {
	return Vec3(v1.buf[0] - v2.buf[0], v1.buf[1] - v2.buf[1], v1.buf[2] - v2.buf[2]);
}

inline Vec3 operator*(const Vec3& v1, const Vec3& v2) {
	return Vec3(v1.buf[0] * v2.buf[0], v1.buf[1] * v2.buf[1], v1.buf[2] * v2.buf[2]);
}

inline Vec3 operator*(const Vec3& v, float f) {
	return Vec3(v.buf[0] * f, v.buf[1] * f, v.buf[2] * f);
}

inline Vec3 operator*(float f, const Vec3& v) {
	return v * f;
}

inline Vec3 operator/(const Vec3& v, float f) {
	return v * (1/f);
}

inline float dot(const Vec3& v1, const Vec3& v2) {
	return v1.buf[0] * v2.buf[0] + v1.buf[1] * v2.buf[1] + v1.buf[2] * v2.buf[2];
}

inline Vec3 cross(const Vec3& v1, const Vec3& v2) {
	return Vec3(
		v1.buf[1] * v2.buf[2] - v1.buf[2] * v2.buf[1],
		v1.buf[2] * v2.buf[0] - v1.buf[0] * v2.buf[2],
		v1.buf[0] * v2.buf[1] - v1.buf[1] * v2.buf[0]
	);
}

inline Vec3 unit_vector(const Vec3& v) {
	return v / v.length();
}

inline Vec3 random_unit_vector() {
	while (true) {
		Vec3 p = Vec3::random(-1, 1);
		float length_squared = p.length_squared();
		if (1e-18 < length_squared && length_squared <= 1) {
			return p / sqrt(length_squared);
		}
	}
}

inline Vec3 random_in_unit_disk() {
	while (1) {
		Vec3 p = Vec3(random_float(-1, 1), random_float(-1, 1), 0);
		if (p.length_squared() < 1) {
			return p;
		}
	}
}

inline Vec3 random_on_hemisphere(const Vec3& normal) {
	Vec3 unit_vector = random_unit_vector();
	return unit_vector * (dot(unit_vector, normal) > 0 ? 1 : -1);
}

inline Vec3 reflect(const Vec3& v, const Vec3& n) {
	return v - 2 * dot(v, n) * n;
}

inline Vec3 refract(const Vec3& v, const Vec3& n, float refract_ratio) {
	float cos_theta = std::fmin(dot(-v, n), 1.0f);
	Vec3 perpendicular = refract_ratio * (v + cos_theta * n);
	Vec3 parallel = -std::sqrt(std::abs(1.0 - perpendicular.length_squared())) * n;
	return perpendicular + parallel;
}

using Point3 = Vec3;