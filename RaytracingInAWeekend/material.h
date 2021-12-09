#pragma once

#include "color.h"
#include "rtweekend.h"
#include "hittable.h"

struct hit_record;

class material {
	public:
		virtual bool scatter(
			const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const = 0;
};

class lambertian : public material {
	public:
		lambertian(const color &a) : albedo(a) {}

		virtual bool scatter(
			const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered
		) const override {
			auto scatter_direction = rec.normal + random_unit_vector();

			if (scatter_direction.magnitude_squared() < 1e-16)
			{
				scatter_direction = rec.normal;
			}

			scattered = ray(rec.point, scatter_direction);
			attenuation = albedo;
			return true;
		}

	public:
		color albedo;
};

class metal : public material {
	public:
		metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

		virtual bool scatter(
			const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered
		) const override {
			auto scatter_direction = reflect(r_in.direction(), rec.normal);
			scattered = ray(rec.point, scatter_direction + fuzz * random_in_unit_sphere());
			attenuation = albedo;
			return (dot(scatter_direction, rec.normal) > 0);
		}
	public:
		color albedo;
		double fuzz;
};

class dialectric : public material {
	public: 
		dialectric(double index_of_refraction) : index_of_refraction(index_of_refraction) {}

		virtual bool scatter(
			const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered
		) const override {
			attenuation = color(1.0, 1.0, 1.0);
			double refraction_ratio = rec.front_face ? (1.0 / index_of_refraction) : index_of_refraction;

			double3 unit_direction = normalize(r_in.direction());
			double cos_theta = dot(-unit_direction, rec.normal);
			double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

			bool cannot_refract = refraction_ratio * sin_theta > 1.0;
			double3 direction;

			if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
			{
				direction = reflect(unit_direction, rec.normal);
			}
			else
			{
				direction = refract(unit_direction, rec.normal, refraction_ratio);
			}

			scattered = ray(rec.point, direction);
			return true;
		}
	public:
		double index_of_refraction;

	private:
		static double reflectance(double cosine, double refraction_index) {
			// Schlick's approximation
			auto r0 = (1 - refraction_index) / (1 + refraction_index);
			r0 = r0 * r0;
			return r0 + (1 - r0) * std::pow((1 - cosine), 5);
		}
};