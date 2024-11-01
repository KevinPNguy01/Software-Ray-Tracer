#pragma once

#include <limits>

class Range {
public:
	float start, end;

	Range() : start(std::numeric_limits<float>::infinity()), end(-std::numeric_limits<float>::infinity()) {}
	Range(float start, float end) : start(start), end(end) {}

	float size() const {
		return end - start;
	}

	bool contains(float num) const {
		return start <= num && num <= end;
	}

	bool surrounds(float num) const {
		return start < num && num < end;
	}

	static const Range empty;
	static const Range universe;
};