#pragma once

#include "hittable.hpp"

#include <memory>
#include <vector>

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


	bool hit(const Ray& r, float tmin, float tmax, Hit& hit_obj) const override {
		Hit hit_temp;
		bool did_hit = false;
		float closest = tmax;

		for (const shared_ptr<Hittable>& object : objects) {
			if (object->hit(r, tmin, closest, hit_temp)) {
				did_hit = true;
				closest = hit_temp.t;
				hit_obj = hit_temp;
			}
		}

		return did_hit;
	}
};