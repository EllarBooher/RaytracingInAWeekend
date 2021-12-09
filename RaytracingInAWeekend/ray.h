#pragma once

#include "double3.h"

class ray {
	public:
		ray() {}
		ray(const point3& origin, const point3& direction) 
			: orig(origin), dir(direction)
		{}

		point3 origin() const { return orig; }
		double3 direction() const { return dir; }

		point3 at(double t) const {
			return orig + t * dir;
		}

	public:
		point3 orig;
		double3 dir;
};
