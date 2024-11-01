#pragma once

#include <cmath>
#include <iostream>

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

using point3 = Vec3;