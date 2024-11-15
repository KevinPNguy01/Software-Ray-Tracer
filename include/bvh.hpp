#pragma once

#include <algorithm>

#include "aabb.hpp"
#include "util.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"

class BVH_Node : public Hittable {
public:
	BVH_Node(HittableList list) : BVH_Node(list.objects, 0, list.objects.size()) {}

	BVH_Node(std::vector<shared_ptr<Hittable>> objects, size_t start, size_t end) {
		bbox = AABB::empty;
		for (size_t object_index = start; object_index < end; object_index++) {
			bbox = AABB(bbox, objects[object_index]->bounding_box());
		}

		int axis = bbox.longest_axis();
		auto comparator = [axis](const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
			Range a_axis_range = a->bounding_box().axis_range(axis);
			Range b_axis_range = b->bounding_box().axis_range(axis);
			return a_axis_range.start < b_axis_range.start;
		};
		size_t object_span = end - start;

		if (object_span == 1) {
			left = right = objects[start];
		}
		else if (object_span == 2) {
			left = objects[start];
			right = objects[start + 1];
		}
		else {
			std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);
			int mid = start + object_span / 2;
			left = make_shared<BVH_Node>(objects, start, mid);
			right = make_shared<BVH_Node>(objects, mid, end);
		}
	}

	bool hit(const Ray& r, Range t_range, Hit& hit) const override {
		if (!bbox.hit(r, t_range)) {
			return false;
		}

		bool hit_left = left->hit(r, t_range, hit);
		bool hit_right = right->hit(r, Range(t_range.start, hit_left ? hit.t : t_range.end), hit);

		return hit_left || hit_right;
	}

	AABB bounding_box() const override { 
		return bbox; 
	}


private:
	shared_ptr<Hittable> left;
	shared_ptr<Hittable> right;
	AABB bbox;
};