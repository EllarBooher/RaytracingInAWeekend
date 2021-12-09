#pragma once

#include "hittable.h"

class sphere : public hittable {
	public:
		sphere(point3 center, double radius, std::shared_ptr<material> material)
			: center(center), radius(radius), mat_ptr(material) {};

		virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

	public:
		point3 center;
		double radius;
		std::shared_ptr<material> mat_ptr;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	double3 offset_center = r.orig - center;
	//Hardcoded maths, find quadratic discriminant of ray hitting circle

	auto a = r.dir.magnitude_squared();
	auto half_b = dot(offset_center, r.dir);
	auto c = offset_center.magnitude_squared() - radius * radius;

	auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0) return false;
	auto sqrt_discriminant = sqrt(discriminant);

	auto root = (-half_b - sqrt_discriminant) / a;
	if (root < t_min || t_max < root) {
		root = (-half_b + sqrt_discriminant) / a;
		if (root < t_min || t_max < root)
			return false;
	}

	rec.t = root;
	rec.point = r.at(rec.t);

	double3 outward_normal = normalize(rec.point - center);
	rec.set_face_normal(r, outward_normal);

	rec.mat_ptr = mat_ptr;

	return true;
}