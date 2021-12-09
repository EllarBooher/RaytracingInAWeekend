#pragma once

#include <cmath>
#include <ostream>
#include "rtweekend.h"

using std::sqrt;

class double3 {
	public:
		double3() : e{0,0,0} {}
		double3(double e0, double e1, double e2) : e{ e0,e1,e2 } {}

		inline static double3 random(double min, double max) {
			return double3(random_double(min, max), random_double(min, max), random_double(min, max));
		}

		double x() const { return e[0]; }
		double y() const { return e[1]; }
		double z() const { return e[2]; }

		double3 operator-() const { return double3(-e[0], -e[1], -e[2]); }
		double operator[](int i) const { return e[i]; }
		double& operator[](int i) { return e[i]; }

		double3& operator+=(const double3& f) {
			e[0] += f.e[0];
			e[1] += f.e[1];
			e[2] += f.e[2];
			return *this;
		}

		double3& operator*=(const double t) {
			e[0] *= t;
			e[1] *= t;
			e[2] *= t;
			return *this;
		}

		double3& operator/=(const double t) {
			return *this *= 1 / t;
		}

		double magnitude() const {
			return sqrt(magnitude_squared());
		}

		double magnitude_squared() const {
			return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
		}

	public:
		double e[3];
};

using point3 = double3;

inline std::ostream& operator<<(std::ostream &out, const double3 &v) {
	return out << '(' << v.e[0] << ',' << v.e[1] << ',' << v.e[2] << ')';
}

inline double3 operator+(const double3 &u, const double3 &v) {
	return double3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline double3 operator-(const double3 &u, const double3 &v) {
	return double3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline double3 operator*(const double3 &u, const double3 &v) {
	return double3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline double3 operator*(const double t, const double3 &v) {
	return double3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline double3 operator/(const double3 &v, const double t) {
	return (1 / t) * v;
}

inline double dot(const double3 &u, const double3 &v) {
	return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}
inline double3 cross(const double3 &u, const double3& v) {
	return double3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline double3 normalize(const double3 &v) {
	return v / v.magnitude();
}

inline double3 random_in_unit_sphere() {
	auto radius = std::sqrt(random_double());
	auto theta = random_double() * 2 * pi;
	auto phi = random_double() * 2 * pi;

	auto x = radius * std::sin(phi) * std::cos(theta);
	auto y = radius * std::sin(phi) * std::sin(theta);
	auto z = radius * std::cos(phi);

	return double3(x, y, z);
}

inline double3 random_in_unit_disk() {
	auto radius = std::sqrt(random_double());
	auto theta = random_double() * 2 * pi;

	auto x = radius * std::cos(theta);
	auto y = radius * std::sin(theta);
	auto z = 0;

	return double3(x, y, z);
}

inline double3 random_unit_vector() {
	auto radius = 1;
	auto theta = random_double() * 2 * pi;
	auto phi = random_double() * 2 * pi;

	auto x = radius * std::sin(phi) * std::cos(theta);
	auto y = radius * std::sin(phi) * std::sin(theta);
	auto z = radius * std::cos(phi);

	return double3(x, y, z);
}

double3 random_in_unit_sphere_rejection() {
	while (true)
	{
		auto random_double3 = double3(random_double(-1,1), random_double(-1,1), random_double(-1,1));
		if (random_double3.magnitude_squared() >= 1) continue;
		return random_double3;
	}
}

inline double3 reflect(const double3 &v, const double3 &normal) {
	return v - 2 * dot(v, normal) * normal;
}

inline double3 refract(const double3& v, const double3& normal, double etai_over_etat) {
	double3 unit_normal = normalize(normal);

	//TODO: make more efficient if needed
	double3 r_out_perp = etai_over_etat * (v + -dot(v, unit_normal) * unit_normal);
	double3 r_out_parallel = -std::sqrt(1 - r_out_perp.magnitude_squared() / v.magnitude_squared()) * unit_normal;
	return r_out_perp + r_out_parallel;
}