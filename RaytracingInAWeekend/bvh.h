#pragma once
#include "hittable.h"
#include <vector>
#include <memory>

class bvh_node : public hittable {
public:
	bvh_node() {};
	bvh_node(
		const std::vector<std::shared_ptr<hittable>> &objects, 
		size_t start, 
		size_t end, 
		double time0, 
		double time1);

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
	virtual bool get_bounding_box(double time0, double time1, aabb& output_box) const override;

public:
	std::shared_ptr<hittable> left;
	std::shared_ptr<hittable> right;
	aabb box;
};
