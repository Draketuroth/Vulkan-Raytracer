cmake_minimum_required(VERSION 3.11-3.18)

set(EXE_NAME ${PROJECT_NAME})
set(VULKAN_SDK_DIR "D:/VulkanSDK/1.2.170.0")

# Include files
include_directories( 
.
./vulkan 
./core 
${VULKAN_SDK_DIR}/Include
${CMAKE_SOURCE_DIR}/external/include)

# Source Groups
set(CORE_FILES
	core/main.cpp
)

set(VULKAN_FILES
)

source_group("Core" FILES ${CORE_FILES})
source_group("Vulkan" FILES ${VULKAN_FILES})

# Target 
add_executable(${EXE_NAME} 
	${CORE_FILES}
	${VULKAN_FILES}
)

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
    "${VULKAN_SDK_DIR}/Lib;${CMAKE_SOURCE_DIR}/external/$<CONFIG>/lib"
)

# Flags
target_compile_features(${EXE_NAME} PUBLIC cxx_std_20)
