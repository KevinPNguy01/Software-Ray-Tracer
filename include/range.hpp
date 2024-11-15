#pragma once

#include <cmath>

#include "util.hpp"

class Range {
public:
	float start, end;

	Range() : start(infinity), end(infinity) {}
	Range(float start, float end) : start(start), end(end) {}

	Range(const Range& a, const Range& b) {
		// Create the interval tightly enclosing the two input intervals.
		start = std::fmin(a.start, b.start);
		end = std::fmax(a.end, b.end);
	}

	float size() const {
		return end - start;
	}

	bool contains(float num) const {
		return start <= num && num <= end;
	}

	bool surrounds(float num) const {
		return start < num && num < end;
	}

	float clamp(float x) const {
		if (x < start) return start;
		if (x > end) return end;
		return x;
	}

	Range expand(float delta) const {
		float padding = delta / 2;
		return Range(start - delta, end + delta);
	}

	static const Range empty;
	static const Range universe;
};