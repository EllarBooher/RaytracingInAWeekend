#pragma once

#include "double3.h"
#include <iostream>
#include <tuple>

using color = double3;

class rgb {
	public:
		rgb() : e{ 0,0,0 } {}
		rgb(uint8_t red, uint8_t green, uint8_t blue) : e{ red, green, blue } {}

		uint8_t r() const { return e[0]; }
		uint8_t g() const { return e[1]; }
		uint8_t b() const { return e[2]; }

	public:
		uint8_t e[3];
};

rgb color_to_rgb(const color& pixel_color) {
	return rgb(	static_cast<int>(255.999 * pixel_color.x()),
				static_cast<int>(255.999 * pixel_color.y()),
				static_cast<int>(255.999 * pixel_color.z()));
}

void write_color(std::ostream &out, const color &pixel_color) {
	rgb pixel_rgb = color_to_rgb(pixel_color);
	out << 'r' << pixel_rgb.r() << ' '
		<< 'g' << pixel_rgb.g() << ' '
		<< 'b' << pixel_rgb.b() << '\n';
}