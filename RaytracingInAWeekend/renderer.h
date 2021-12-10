#pragma once

#include <vector>
#include "sphere.h"
#include "camera.h"

class renderer {
public:
	renderer();

	void render() const;

private:
	camera main_camera;
	std::vector<sphere> scene;
	const uint16_t max_bounce_depth;
	const double aspect_ratio;
};