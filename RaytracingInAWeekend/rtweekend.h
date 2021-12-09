#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>

// Constants

const double infinity_double = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility functions

inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

/// <summary>
/// Returns a random double within [0,1).
/// </summary>
inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

/// <summary>
/// Returns a random double within [min,max).
/// </summary>
inline double random_double(double min, double max) {
	return min + (max - min) * random_double();
}

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}