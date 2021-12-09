# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("RaytracingInAWeekend")

enable_testing()

# Include sub-projects.
add_subdirectory ("RaytracingInAWeekend")
add_subdirectory ("UnitTests")