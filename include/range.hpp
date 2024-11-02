#pragma once

#include "util.hpp"

class Range {
public:
	float start, end;

	Range() : start(infinity), end(infinity) {}
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

	float clamp(float x) const {
		if (x < start) return start;
		if (x > end) return end;
		return x;
	}

	static const Range empty;
	static const Range universe;
};