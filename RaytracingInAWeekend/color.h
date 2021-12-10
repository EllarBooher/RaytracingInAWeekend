#pragma once

#include "double3.h"
#include <iostream>
#include <tuple>

using color = double3;

class rgb {
public:
	rgb();
	rgb(uint8_t red, uint8_t green, uint8_t blue);

public:
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

namespace color_helpers
{
	inline color rgb_to_color(uint8_t r, uint8_t g, uint8_t b) {
		return color(double(r) / 256, double(g) / 256, double(b) / 256);
	}

	inline rgb color_to_rgb(const color& pixel_color) {
		return rgb(static_cast<int>(255.999 * pixel_color.x()),
			static_cast<int>(255.999 * pixel_color.y()),
			static_cast<int>(255.999 * pixel_color.z()));
	}

	inline void write_color(std::ostream& out, const color& pixel_color) {
		rgb pixel_rgb = color_to_rgb(pixel_color);
		out << 'r' << pixel_rgb.r << ' '
			<< 'g' << pixel_rgb.g << ' '
			<< 'b' << pixel_rgb.b << '\n';
	}
}