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
	Core/Utilities/Glm.h
)

set(VULKAN_DESCRIPTOR_FILES
	Vulkan/Descriptor/DescriptorSetManager.h
	Vulkan/Descriptor/DescriptorSetManager.cpp
	Vulkan/Descriptor/DescriptorPool.h
	Vulkan/Descriptor/DescriptorPool.cpp
	Vulkan/Descriptor/DescriptorSetLayout.h
	Vulkan/Descriptor/DescriptorSetLayout.cpp
	Vulkan/Descriptor/DescriptorBinding.h
	Vulkan/Descriptor/DescriptorSets.h
	Vulkan/Descriptor/DescriptorSets.cpp
)

set(VULKAN_FILES
	Vulkan/Application.h
	Vulkan/Application.cpp
	Vulkan/Core.h
	Vulkan/Core.cpp
	Vulkan/Instance.h
	Vulkan/Instance.cpp
	Vulkan/Version.h
	Vulkan/Enumerate.h
	Vulkan/Surface.h
	Vulkan/Surface.cpp
	Vulkan/Device.h
	Vulkan/Device.cpp
	Vulkan/CommandPool.h
	Vulkan/CommandPool.cpp
	Vulkan/SwapChain.h
	Vulkan/SwapChain.cpp
	Vulkan/ImageView.h
	Vulkan/ImageView.cpp
	Vulkan/Image.h
	Vulkan/Image.cpp
	Vulkan/DepthBuffer.h
	Vulkan/DepthBuffer.cpp
	Vulkan/DeviceMemory.h
	Vulkan/DeviceMemory.cpp
	Vulkan/Buffer.h
	Vulkan/Buffer.cpp
	Vulkan/CommandBuffers.h
	Vulkan/CommandBuffers.cpp
	Vulkan/SingleTimeCommands.h
	Vulkan/Fence.h
	Vulkan/Fence.cpp
	Vulkan/Semaphore.h
	Vulkan/Semaphore.cpp
	Vulkan/GraphicsPipeline.h
	Vulkan/GraphicsPipeline.cpp
	Vulkan/RenderPass.h
	Vulkan/RenderPass.cpp
	Vulkan/PipelineLayout.h
	Vulkan/PipelineLayout.cpp
	Vulkan/Sampler.h
	Vulkan/Sampler.cpp
	Vulkan/BufferUtilities.h
	Vulkan/ShaderModule.h
	Vulkan/ShaderModule.cpp
	Vulkan/FrameBuffer.h
	Vulkan/FrameBuffer.cpp
)

set(VULKAN_RESOURCES_FILES
	Vulkan/Resources/UniformBuffer.h
	Vulkan/Resources/UniformBuffer.cpp
	Vulkan/Resources/Scene.h
	Vulkan/Resources/Scene.cpp
	Vulkan/Resources/Model.h
	Vulkan/Resources/Model.cpp
	Vulkan/Resources/Vertex.h
	Vulkan/Resources/Material.h
	Vulkan/Resources/Procedural.h
	Vulkan/Resources/Sphere.h
	Vulkan/Resources/Texture.h
	Vulkan/Resources/Texture.cpp
	Vulkan/Resources/TextureImage.h
	Vulkan/Resources/TextureImage.cpp
)

set(VULKAN_DATA_FILES
	Vulkan/Data/Vertex.h
)

set(VULKAN_DEBUG_FILES
    Vulkan/Debug/Info.h
	Vulkan/Debug/Info.cpp
	Vulkan/Debug/Messenger.h
	Vulkan/Debug/Messenger.cpp
	Vulkan/Debug/Utilities.h
	Vulkan/Debug/Utilities.cpp
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
source_group("Vulkan/Debug" FILES ${VULKAN_DEBUG_FILES})
source_group("Vulkan/Resources" FILES ${VULKAN_RESOURCES_FILES})
source_group("Vulkan/Descriptor" FILES ${VULKAN_DESCRIPTOR_FILES})

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
	${VULKAN_DESCRIPTOR_FILES}
	${VULKAN_RAYTRACING_FILES}
	${VULKAN_DEBUG_FILES}
	${VULKAN_RESOURCES_FILES}
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
