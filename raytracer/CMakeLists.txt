cmake_minimum_required(VERSION 3.11-3.18)
include(../PrecompiledHeader.cmake)

set(EXE_NAME ${PROJECT_NAME})
set(VULKAN_SDK_DIR "D:/VulkanSDK/1.2.170.0")

# Include files
include_directories( 
.
./Vulkan
./Core
./Event
./Platform
${VULKAN_SDK_DIR}/Include
${CMAKE_SOURCE_DIR}/External/include)

# Source Groups
set(PRE_COMPILE_FILES
	pch.cpp
)

set(PLATFORM_FILES
	Platform/StandardTypes.h
)

set(CORE_FILES
	Core/main.cpp
	Core/WindowProperties.h
	Core/Raytracer.h
	Core/Raytracer.cpp
	Core/Window.h
	Core/Window.cpp
)

set(UTILITIES_FILES
	Core/Utilities/StbImage.h
	Core/Utilities/StbImage.cpp
)

set(VULKAN_FILES
	Vulkan/Application.h
	Vulkan/Application.cpp
	Vulkan/VulkanCore.h
	Vulkan/VulkanCore.cpp
	Vulkan/Instance.h
	Vulkan/Instance.cpp
	Vulkan/Version.h
	Vulkan/Enumerate.h
	Vulkan/VulkanInfo.h
	Vulkan/VulkanInfo.cpp
	Vulkan/DebugMessenger.h
	Vulkan/DebugMessenger.cpp
	Vulkan/Surface.h
	Vulkan/Surface.cpp
)

set(VULKAN_RAYTRACING_FILES
	Vulkan/Raytracing/Application.h
	Vulkan/Raytracing/Application.cpp
)

source_group("" FILES ${PRE_COMPILE_FILES})
source_group("Platform" FILES ${PLATFORM_FILES})
source_group("Core" FILES ${CORE_FILES})
source_group("Core/Utilities" FILES ${UTILITIES_FILES})
source_group("Vulkan" FILES ${VULKAN_FILES})
source_group("Vulkan/Raytracing" FILES ${VULKAN_RAYTRACING_FILES})

if(MSVC)
    add_compile_options(
        $<$<CONFIG:>:/MT>
        $<$<CONFIG:Debug>:/MTd>
        $<$<CONFIG:Release>:/MT>
    )
endif()

# Target 
add_executable(${EXE_NAME} 
    ${PRE_COMPILE_FILES}
	${PLATFORM_FILES}
	${UTILITIES_FILES}
	${EVENT_FILES}
	${CORE_FILES}
	${VULKAN_FILES}
	${VULKAN_RAYTRACING_FILES}
)

add_precompiled_header(${EXE_NAME} pch.h SOURCE_CXX pch.cpp FORCEINCLUDE)

# External libraries
string(CONCAT ADDITIONAL_LIBRARY_DEPENDENCIES
    "$<$<CONFIG:Debug>:"
		"glfw3;"
		"imguid;"
		"vulkan-1;"
    ">"
    "$<$<CONFIG:Release>:"
		"glfw3;"
		"imgui;"
		"vulkan-1;"
    ">"
)
target_link_libraries(${EXE_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${EXE_NAME} PUBLIC
    "${VULKAN_SDK_DIR}/Lib;${CMAKE_SOURCE_DIR}/External/$<CONFIG>/lib"
)

# Flags
target_compile_features(${EXE_NAME} PUBLIC cxx_std_20)

add_custom_command(
	TARGET ${EXE_NAME} 
	PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/Assets/" $<TARGET_FILE_DIR:${EXE_NAME}>/Assets
)
