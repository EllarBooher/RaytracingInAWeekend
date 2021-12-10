#pragma once

#include "double3.h"

class ray {
	public:
		ray() {}
		ray(const point3& origin, const point3& direction) 
			: orig(origin), dir(direction)
		{
			dir_inverse = double3(1, 1, 1) / dir;
		}

		inline point3 origin() const { return orig; }
		inline double3 direction() const { return dir; }
		inline double3 direction_inverse() const { return dir_inverse; }

		point3 at(double t) const {
			return orig + t * dir;
		}

	private:
		point3 orig;
		double3 dir;
		double3 dir_inverse;
};
