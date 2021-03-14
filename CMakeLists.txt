cmake_minimum_required(VERSION 3.11-3.18)

set(PROJECT_NAME "Raytracer")

project(
    ${PROJECT_NAME}
	LANGUAGES C CXX
    VERSION 0.1
    DESCRIPTION "A Vulkan Raytracer")
	

add_subdirectory(raytracer)
set_property (DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})