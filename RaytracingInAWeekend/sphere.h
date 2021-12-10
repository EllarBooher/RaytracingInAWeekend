#pragma once

#include "hittable.h"

class sphere : public hittable {
	public:
		sphere(point3 center, double radius, std::shared_ptr<material> material);

		virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

	public:
		point3 center;
		double radius;
		std::shared_ptr<material> mat_ptr;
};