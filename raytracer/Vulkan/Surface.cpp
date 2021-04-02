
#include "Vulkan/Surface.h"

#include "Vulkan/Instance.h"

#include "Core/Window.h"

namespace Vulkan 
{
    Surface::Surface(const Instance& instanceIn) :
        instance(instanceIn)
    {
        CheckVKResult(glfwCreateWindowSurface(instance.getHandle(), instance.getWindow().getHandle(), nullptr, &surface), "Create Window Surface");
    }
    Surface::~Surface()
    {
        if (surface != nullptr) 
        {
            vkDestroySurfaceKHR(instance.getHandle(), surface, nullptr);
            surface = nullptr;
        }
    }
}