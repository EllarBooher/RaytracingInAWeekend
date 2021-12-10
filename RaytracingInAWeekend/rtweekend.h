#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <chrono>

namespace utility
{
    // Constants

    const double infinity_double = std::numeric_limits<double>::infinity();
    const double pi = 3.1415926535897932385;

    // Utility functions

    inline double degrees_to_radians(double degrees) {
        return degrees * pi / 180.0;
    }

    class fastrng {
    public:
        fastrng(uint64_t seed) : seed(seed) {};
        inline double get() {
            seed = (214013 * seed + 2531011);
            return double((seed >> 16) & 0x7FFF) / 0x7FFF;
        }
    private:
        uint64_t seed;
    };

    /// <summary>
    /// Returns a random double within [0,1).
    /// </summary>
    inline double random_double() {
        static fastrng rng(std::chrono::system_clock::now().time_since_epoch().count());
        return rng.get();
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
}