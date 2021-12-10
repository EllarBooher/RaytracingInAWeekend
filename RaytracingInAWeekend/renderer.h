#pragma once

#include <vector>
#include "sphere.h"
#include "camera.h"
#include "bvh.h"

class renderer {
public:
	renderer();

	void render() const;

private:

	camera main_camera;
	std::vector<std::shared_ptr<hittable>> scene;
	const uint16_t max_bounce_depth;
	const double aspect_ratio;
	bvh_node bvh_tree;
};