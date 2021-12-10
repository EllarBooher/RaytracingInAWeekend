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

        inline double get_double() {
            return double(get_uint64()) / 0x7FFF;
        }

        inline uint64_t get_uint64() {
            seed = (214013 * seed + 2531011);
            return (seed >> 16) & 0x7FFF;
        }

    private:
        uint64_t seed;
    };

    /// <summary>
    /// Returns a random double within [0,1).
    /// </summary>
    inline double random_double() {
        static fastrng rng(std::chrono::system_clock::now().time_since_epoch().count());
        return rng.get_double();
    }
    /// <summary>
    /// Returns a random double within [min,max).
    /// </summary>
    inline double random_double(double min, double max) {
        return min + (max - min) * random_double();
    }

    inline uint64_t random_uint64(uint64_t min, uint64_t max) {
        static fastrng rng(std::chrono::system_clock::now().time_since_epoch().count());
        return rng.get_uint64();
    }

    inline double clamp(double x, double min, double max) {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }
}