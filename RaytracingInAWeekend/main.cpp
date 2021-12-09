// RaytracingInAWeekend.cpp : Defines the entry point for the application.
//

#include "color.h"
#include "ray.h"
#include "sphere.h"
#include "rtweekend.h"
#include "camera.h"
#include "material.h"
#include <vector>
#include <iostream>
#include <array>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../STBImage/stb_image_write.h"

/// <summary>
/// Returns the smallest parameter of the ray that hits this sphere.
/// </summary>
double hit_basic_sphere(const point3& center, double radius, const ray& r)
{
	double3 offset_center = r.origin() - center;

	//Hardcoded maths, find quadratic discriminant of ray hitting circle
	auto a = r.direction().magnitude_squared();
	auto half_b = dot(offset_center, r.direction());
	auto c = offset_center.magnitude_squared() - radius * radius;
	auto discriminant = half_b*half_b - a * c;

	if (discriminant < 0) { return -1; }
	else 
	{
		//Quadratic formula
		return (-half_b - sqrt(discriminant)) / a;
	}
}

color ray_color(const ray &r, const std::vector<sphere> &spheres, int depth) 
{
	//Check for collision with objects

	//Bounced too many times, so all light is gone
	if (depth <= 0) {
		return color(0, 0, 0);
	}

	//Bounds for where we are searching
	double t_max = infinity_double;
	double t_min = 0.0001;

	hit_record record;
	bool hit_anything = false;
	auto closest_t_so_far = t_max;

	for (const sphere &object : spheres)
	{
		if (object.hit(r, t_min, closest_t_so_far, record))
		{
			hit_anything = true;
			closest_t_so_far = record.t;
		}
	}

	if (hit_anything)
	{
		ray scattered;
		color attenuation;

		if (record.mat_ptr->scatter(r, record, attenuation, scattered))
		{
			return attenuation * ray_color(scattered, spheres, depth - 1);
		}
		return color(0, 0, 0);
	}

	//Background collision

	double3 unit_direction = normalize(r.direction());
	//map y direction (vertical angle) from 0.0 to 1.0, with 0 mapping to 0.5
	auto t = 0.5 * (unit_direction.y() + 1.0);
	//lerp between white and some bluish color
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

std::vector<sphere> random_scene()
{
	auto material_ground = std::make_shared<lambertian>(color(0.6, 0.6, 0.4));
	auto material_center = std::make_shared<dialectric>(1.5);
	auto material_left = std::make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
	auto material_right = std::make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);
	auto material_front = std::make_shared<lambertian>(color(0.1, 0.2, 0.5));

	std::vector<sphere> spheres{};
	spheres.push_back(sphere(point3(0, -1000, 0), 1000, material_ground));

	for (int a = -11; a < 11; ++a)
	{
		for (int b = -11; b < 11; ++b)
		{
			auto choose_mat = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			std::shared_ptr<material> sphere_material;
			if (choose_mat < 0.8) {
				//Diffuse
				//Favor darker colors
				auto albedo = color::random(0, 1) * color::random(0, 1);
				sphere_material = std::make_shared<lambertian>(albedo);
				spheres.push_back(sphere(center, 0.2, sphere_material));
			}
			else if (choose_mat < 0.95) {
				//Metal

				auto albedo = color::random(0.5, 1);
				auto fuzz = random_double(0, 0.5);
				sphere_material = std::make_shared<metal>(albedo, fuzz);
				spheres.push_back(sphere(center, 0.2, sphere_material));
			}
			else
			{
				sphere_material = std::make_shared<dialectric>(1.5);
				spheres.push_back(sphere(center, 0.2, sphere_material));
			}
		}
	}

	auto material1 = std::make_shared<dialectric>(1.5);
	auto material2 = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
	auto material3 = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);

	spheres.push_back(sphere(point3(0, 1, 0), 1.0, material1));
	spheres.push_back(sphere(point3(-4, 1, 0), 1.0, material2));
	spheres.push_back(sphere(point3(4, 1, 0), 1.0, material3));

	return spheres;
}

int main()
{
	const auto aspect_ratio = 16.0 / 9.0;

	// Image Setup

	const uint16_t image_width = 100;
	const uint16_t image_height = static_cast<uint16_t>(image_width / aspect_ratio);

	const uint16_t bytes_per_RGB_pixel = 3;
	const uint16_t stride_in_bytes = bytes_per_RGB_pixel * image_width;
	const double samples_per_pixel = 25;
	const double sqrt_samples_per_pixel = 5;

	const int max_bounce_depth = 50;

	auto spheres = random_scene();

	// Camera
	point3 lookfrom(13, 2, 3);
	point3 lookat(0, 0, 0);
	double3 up(0, 1, 0);
	auto dist_to_focus = 10;
	auto aperture = 0.1;
	double vertical_fov = 20;


	camera main_camera(lookfrom, lookat, up, vertical_fov, aspect_ratio, aperture, dist_to_focus);

	// Render

	std::vector<rgb> pixelsRGB{};

	for (double j = image_height - 1; j >= 0; --j) {
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (double i = 0; i < image_width; ++i) {
			
			color pixel_color(0, 0, 0);
			for (double sj = 0; sj < sqrt_samples_per_pixel; ++sj) {
				for (double si = 0; si < sqrt_samples_per_pixel; ++si) {
					auto u = (double(i) + (si / sqrt_samples_per_pixel)) / (image_width - 1);
					auto v = (double(j) + (sj / sqrt_samples_per_pixel)) / (image_height - 1);

					ray r = main_camera.get_ray(u, v);

					pixel_color += ray_color(r, spheres, max_bounce_depth);
				}
			} 
			//gama correction
			pixel_color /= samples_per_pixel;
			auto r = sqrt(pixel_color.x());
			auto g = sqrt(pixel_color.y());
			auto b = sqrt(pixel_color.z());
			pixel_color = color(r, g, b);

			/*
			auto u = double(i) / (image_width - 1);
			auto v = double(j) / (image_height - 1);

			ray r = main_camera.get_ray(u, v);

			auto pixel_color = ray_color(r, spheres);*/

			rgb pixel_rgb = color_to_rgb(pixel_color);

			pixelsRGB.push_back(pixel_rgb);
		}
	}



	stbi_write_png("Result.png", image_width, image_height, 3, pixelsRGB.data(), stride_in_bytes);
}