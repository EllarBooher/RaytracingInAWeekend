#pragma once

#include "rtweekend.h"
#include "double3.h"

class aabb {
public:
	aabb() {}
	aabb(const point3 &a, const point3 &b) {}

	inline point3 minimum() const { return min; };
	inline point3 maximum() const { return max; };

	inline bool hit(const ray& r, double t_min, double t_max) const {
		for (int a = 0; a < 3; ++a)
		{
			auto inverse_direction = 1.0f / r.direction()[a];
			auto t0 = (minimum()[a] - r.origin()[a]) * inverse_direction;
			auto t1 = (maximum()[a] - r.origin()[a]) * inverse_direction;
			if (inverse_direction < 0.0f)
			{
				std::swap(t0, t1);
			}
			t_min = t0 > t_min ? t0 : t_min;
			t_max = t1 < t_max ? t1 : t_max;
			if (t_max <= t_min)
			{
				return false;
			}
		}
		return true;
	}

	static aabb surrounding_box(const aabb& box0, const aabb& box1) {
		double3 smallest_corner(
			std::fmin(box0.minimum().x(), box1.minimum().x()),
			std::fmin(box0.minimum().y(), box1.minimum().y()),
			std::fmin(box0.minimum().z(), box1.minimum().z()));

		double3 biggest_corner(
			std::fmax(box0.maximum().x(), box1.maximum().x()),
			std::fmax(box0.maximum().y(), box1.maximum().y()),
			std::fmax(box0.maximum().z(), box1.maximum().z()));

		return aabb(smallest_corner, biggest_corner);
	}

private:
	point3 min;
	point3 max;
};