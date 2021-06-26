#ifndef VULKAN_CORE_H
#define VULKAN_CORE_H

#define NOMINMAX
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#undef APIENTRY

namespace Vulkan 
{
    void CheckVKResult(VkResult result, const Platform::Type::Char* operation);
    const char* VKResultToString(VkResult result);
}

#endif