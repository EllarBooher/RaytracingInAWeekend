#pragma once

#include <cmath>
#include <ostream>
#include "rtweekend.h"

using std::sqrt;

class double3 {
public:
	double3() : e{ 0,0,0 } {};
	double3(double x, double y, double z) : e{ x,y,z } {};

	inline static double3 random(double min, double max) {
		return double3(utility::random_double(min, max), utility::random_double(min, max), utility::random_double(min, max));
	}

	inline double operator[](uint64_t a) const { return e[a]; }
	inline double& operator[](uint64_t a) { return e[a]; }

	inline double3 operator-() const { return double3(-e[0], -e[1], -e[2]); }

	inline double3& operator+=(const double3& f) {
		e[0] += f.e[0];
		e[1] += f.e[1];
		e[2] += f.e[2];
		return *this;
	}

	inline double3& operator*=(const double t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	inline double3& operator/=(const double t) {
		return *this *= 1 / t;
	}

	inline double x() const { return e[0]; };
	inline double y() const { return e[1]; };
	inline double z() const { return e[2]; };

	inline double double3::magnitude() const {
		return sqrt(magnitude_squared());
	}

	inline double double3::magnitude_squared() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

private:
	double e[3];
};

using point3 = double3;

inline std::ostream& operator<<(std::ostream& out, const double3& v) {
	return out << '(' << v.x() << ',' << v.y() << ',' << v.z() << ')';
}

inline double3 operator+(const double3 &u, const double3 &v) {
	return double3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

inline double3 operator-(const double3 &u, const double3 &v) {
	return double3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

inline double3 operator*(const double3 &u, const double3 &v) {
	return double3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

inline double3 operator*(const double t, const double3 &v) {
	return double3(t * v.x(), t * v.y(), t * v.z());
}

inline double3 operator/(const double3 &v, const double t) {
	return (1 / t) * v;
}

namespace linear_algebra
{
	inline double dot(const double3& u, const double3& v) {
		return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
	}
	inline double3 cross(const double3& u, const double3& v) {
		return double3(u.y() * v.z() - u.z() * v.y(),
			u.z() * v.x() - u.x() * v.z(),
			u.x() * v.y() - u.y() * v.x());
	}

	inline double3 normalize(const double3& v) {
		return v / v.magnitude();
	}

	inline double3 random_in_unit_sphere() {
		auto radius = std::sqrt(utility::random_double());
		auto theta = utility::random_double() * 2 * utility::pi;
		auto phi = utility::random_double() * 2 * utility::pi;

		auto x = radius * std::sin(phi) * std::cos(theta);
		auto y = radius * std::sin(phi) * std::sin(theta);
		auto z = radius * std::cos(phi);

		return double3(x, y, z);
	}

	inline double3 random_in_unit_disk() {
		auto radius = std::sqrt(utility::random_double());
		auto theta = utility::random_double() * 2 * utility::pi;

		auto x = radius * std::cos(theta);
		auto y = radius * std::sin(theta);
		auto z = 0;

		return double3(x, y, z);
	}

	inline double3 random_unit_vector() {
		auto radius = 1;
		auto theta = utility::random_double() * 2 * utility::pi;
		auto phi = utility::random_double() * 2 * utility::pi;

		auto x = radius * std::sin(phi) * std::cos(theta);
		auto y = radius * std::sin(phi) * std::sin(theta);
		auto z = radius * std::cos(phi);

		return double3(x, y, z);
	}

	/// <summary>
	/// This is much slower than the alternative random_in_unit_sphere.
	/// </summary>
	/// <returns></returns>
	inline double3 random_in_unit_sphere_rejection() {
		while (true)
		{
			auto random_double3 = double3(utility::random_double(-1, 1), utility::random_double(-1, 1), utility::random_double(-1, 1));
			if (random_double3.magnitude_squared() >= 1) continue;
			return random_double3;
		}
	}

	inline double3 reflect(const double3& v, const double3& normal) {
		return v - 2 * dot(v, normal) * normal;
	}

	inline double3 refract(const double3& v, const double3& normal, double etai_over_etat) {
		double3 unit_normal = normalize(normal);

		double3 r_out_perp = etai_over_etat * (v + -dot(v, unit_normal) * unit_normal);
		double3 r_out_parallel = -std::sqrt(1 - r_out_perp.magnitude_squared() / v.magnitude_squared()) * unit_normal;
		return r_out_perp + r_out_parallel;
	}
}