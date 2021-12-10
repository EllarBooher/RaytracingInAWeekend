#include "double3.h"
#include "rtweekend.h"

double3::double3() : x(0), y(0), z(0) {};
double3::double3(double x, double y, double z) : x(x), y(y), z(z) {};

double3 double3::random(double min, double max) {
	return double3(utility::random_double(min, max), utility::random_double(min, max), utility::random_double(min, max));
}

double3 double3::operator-() const { return double3(-x, -y, -z); }

double3& double3::operator+=(const double3& f) {
	x += f.x;
	y += f.y;
	z += f.z;
	return *this;
}

double3& double3::operator*=(const double t) {
	x *= t;
	y *= t;
	z *= t;
	return *this;
}

double3& double3::operator/=(const double t) {
	return *this *= 1 / t;
}

double double3::magnitude() const {
	return sqrt(magnitude_squared());
}

double double3::magnitude_squared() const {
	return x * x + y * y + z * z;
}