#ifndef VULKAN_RAYTRACING_APPLICATION_H
#define VULKAN_RAYTRACING_APPLICATION_H

#include "Vulkan/Application.h"

namespace Vulkan 
{
    namespace Raytracing 
    {
        class Application : public Vulkan::Application
        {
        protected:
            Application(const Core::WindowProperties& windowProperties, VkPresentModeKHR presentMode, bool enableValidationLayers);
            ~Application();
        };
    }
}

#endif