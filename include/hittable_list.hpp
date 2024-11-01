#pragma once

#include <memory>
#include <vector>

#include "hittable.hpp"
#include "range.hpp"

using std::make_shared;
using std::shared_ptr;

class HittableList : public Hittable {
public:
	std::vector<shared_ptr<Hittable>> objects;

	HittableList() {}
	HittableList(shared_ptr<Hittable> object) {
		add(object);
	}

	void add(shared_ptr<Hittable> object) {
		objects.push_back(object);
	}
	void clear() {
		objects.clear();
	}


	bool hit(const Ray& r, Range range, Hit& hit_obj) const override {
		Hit hit_temp;
		bool did_hit = false;
		float closest = range.end;

		for (const shared_ptr<Hittable>& object : objects) {
			if (object->hit(r, Range(range.start, closest), hit_temp)) {
				did_hit = true;
				closest = hit_temp.t;
				hit_obj = hit_temp;
			}
		}

		return did_hit;
	}
};