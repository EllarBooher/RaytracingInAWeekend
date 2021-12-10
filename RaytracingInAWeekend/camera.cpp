
#include "double3.h"
#include "ray.h"
#include "camera.h"

camera::camera(
	point3 lookfrom = point3(0, 0, 0),
	point3 lookat = point3(0, 0, -1),
	double3 up = double3(0, 1, 0),
	double vertical_fov = 20,
	double aspect_ratio = 16.0 / 9.0,
	double aperture = 0.1,
	double focus_dist = 1
) {

	auto theta_vertical_fov = utility::degrees_to_radians(vertical_fov);
	auto viewport_height = 2.0 * tan(theta_vertical_fov / 2);
	auto viewport_width = aspect_ratio * viewport_height;

	w = linear_algebra::normalize(lookfrom - lookat);
	u = linear_algebra::normalize(linear_algebra::cross(up, w));
	v = linear_algebra::cross(w, u);

	origin = lookfrom;
	horizontal = focus_dist * viewport_width * u;
	vertical = focus_dist * viewport_height * v;
	lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

	lens_radius = aperture / 2;
}

ray camera::get_ray(double s, double t) const {
	double3 random_from_lens_disk = lens_radius * linear_algebra::random_in_unit_disk();
	double3 offset = random_from_lens_disk.x() * u + random_from_lens_disk.y() * v;

	return ray(
		origin + offset,
		lower_left_corner + s * horizontal + t * vertical - origin - offset
	);
}