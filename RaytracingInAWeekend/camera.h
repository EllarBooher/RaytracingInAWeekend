#pragma once

#include "double3.h"
#include "rtweekend.h"
#include "ray.h"

class camera {
	public:
		camera(
			point3 lookfrom,
			point3 lookat,
			double3 up,
			double vertical_fov,
			double aspect_ratio,
			double aperture,
			double focus_dist
		);

		ray get_ray(double s, double t) const;

	private:
		point3 origin;
		point3 lower_left_corner;
		double3 horizontal;
		double3 vertical;
		double3 u, v, w;
		double lens_radius;
};