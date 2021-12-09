#pragma once

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
	) {

		auto theta_vertical_fov = degrees_to_radians(vertical_fov);
		auto viewport_height = 2.0 * tan(theta_vertical_fov / 2);
		auto viewport_width = aspect_ratio * viewport_height;

		w = normalize(lookfrom - lookat);
		u = normalize(cross(up, w));
		v = cross(w, u);

		origin = lookfrom;
		horizontal = focus_dist * viewport_width * u;
		vertical = focus_dist * viewport_height * v;
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;
	
		lens_radius = aperture / 2;
	}

	ray get_ray(double s, double t) const {
		double3 random_from_lens_disk = lens_radius * random_in_unit_disk();
		double3 offset = random_from_lens_disk.x() * u + random_from_lens_disk.y() * v;

		return ray(
			origin + offset,
			lower_left_corner + s * horizontal + t * vertical - origin - offset
		);
	}

private:
	point3 origin;
	point3 lower_left_corner;
	double3 horizontal;
	double3 vertical;
	double3 u, v, w; //Orthonormal basis for camera
	double lens_radius;
};