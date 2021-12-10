# CMakeList.txt : CMake project for RaytracingInAWeekend, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (RaytracingInAWeekend "main.cpp"  "double3.h" "color.h" "ray.h" "hittable.h" "sphere.h" "camera.h" "material.h" "renderer.h" "color.cpp" "renderer.cpp" "camera.cpp" "double3.cpp" "hittable.cpp" "sphere.cpp")
# TODO: Add tests and install targets if needed.
