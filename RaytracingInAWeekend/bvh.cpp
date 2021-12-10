#include "bvh.h";
#include "rtweekend.h"
#include "hittable.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>

bool bvh_node::get_bounding_box(double time0, double time1, aabb& output_box) const {
	output_box = box;
	return true;
}

bool bvh_node::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	if (!box.hit(r, t_min, t_max))
	{
		return false;
	}

	//TODO: this sort of obscures when exactly the ray hits a real hittable, 
	//so it would be nice to flatten this out
	bool hit_left = left->hit(r, t_min, t_max, rec);
	bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

	return hit_left || hit_right;
}

inline bool box_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b, int axis) {
	aabb box_a;
	aabb box_b;

	if (!a->get_bounding_box(0, 0, box_a) || !b->get_bounding_box(0, 0, box_b))
	{
		std::cerr << "One or more hittables does not have a bounding box.\n";
	}

	return box_a.minimum()[axis] < box_b.minimum()[axis];
}

inline bool box_x_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
	return box_compare(a, b, 0);
}

inline bool box_y_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
	return box_compare(a, b, 1);
}

inline bool box_z_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
	return box_compare(a, b, 2);
}

bvh_node::bvh_node(const std::vector<std::shared_ptr<hittable>>& objects, size_t start, size_t end, double time0, double time1)
{
	auto objects_mutable = objects;

	uint64_t axis = utility::random_uint64(0, 2);

	//TODO: use lambda here
	auto comparator = (axis == 0) ? box_x_compare
		: (axis == 1) ? box_y_compare : box_z_compare;

	size_t object_span = end - start;

	if (object_span == 1) {
		left = right = objects_mutable[start];
	}
	else if (object_span == 2) {
		if (comparator(objects_mutable[start], objects_mutable[start + 1])) {
			left = objects_mutable[start];
			right = objects_mutable[start + 1];
		}
		else {
			left = objects_mutable[start + 1];
			right = objects_mutable[start];
		}
	}
	else
	{
		std::sort(objects_mutable.begin() + start, objects_mutable.begin() + end, comparator);
		auto mid = start + object_span / 2;

		left = std::make_shared<bvh_node>(objects_mutable, start, mid, time0, time1);
		right = std::make_shared<bvh_node>(objects_mutable, mid, end, time0, time1);
	}

	aabb box_left, box_right;

	if (!left->get_bounding_box(0, 0, box_left) || !right->get_bounding_box(0, 0, box_right))
	{
		std::cerr << "One or more hittables does not have a bounding box.\n";
	}

	box = aabb::surrounding_box(box_left, box_right);
}