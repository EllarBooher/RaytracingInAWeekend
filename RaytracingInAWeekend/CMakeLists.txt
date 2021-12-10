# CMakeList.txt : CMake project for RaytracingInAWeekend, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.9)

find_package(OpenMP REQUIRED)
# Add source to this project's executable.
add_executable (RaytracingInAWeekend "main.cpp"  "double3.h" "color.h" "ray.h" "hittable.h" "sphere.h" "camera.h" "material.h" "renderer.h" "color.cpp" "renderer.cpp" "camera.cpp" "bvh.cpp" "sphere.cpp" "aabb.h"  "bvh.h")
set_target_properties(RaytracingInAWeekend PROPERTIES COMPILE_FLAGS "-openmp:experimental")
# TODO: Add tests and install targets if needed.
target_link_libraries(RaytracingInAWeekend PRIVATE OpenMP::OpenMP_CXX)
