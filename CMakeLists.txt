cmake_minimum_required(VERSION 3.11-3.18)

set(PROJECT_NAME "Raytracer")

project(
    ${PROJECT_NAME}
	LANGUAGES C CXX
    VERSION 0.1
    DESCRIPTION "A Vulkan Raytracer")
	
find_package(Vulkan REQUIRED FATAL_ERROR)
	
# Validate the Vulkan installation.
IF (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Could not find Vulkan library!")
ELSE()
    message(STATUS ${Vulkan_LIBRARY})
ENDIF()

# We also require glslangValidator to compile glsl and hlsl shaders into spir-v format.
find_program(Vulkan_GLSLANG_VALIDATOR
	NAMES glslangValidator
	HINTS ENV VULKAN_SDK
	PATH_SUFFIXES bin)
	
if(NOT Vulkan_GLSLANG_VALIDATOR)
	message(FATAL_ERROR "glslangValidator not found!")
endif()

add_subdirectory(Assets)
add_subdirectory(Raytracer)
set_property (DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})