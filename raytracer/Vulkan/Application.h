#ifndef VULKAN_APPLICATION_H
#define VULKAN_APPLICATION_H

#include "Core/WindowProperties.h"

namespace Core { class Window; }

namespace Vulkan { class Instance; }

namespace Vulkan 
{
    class Application 
    {
    public:
        virtual ~Application();

        const std::vector<VkExtensionProperties>& extensions() const;
        const std::vector<VkLayerProperties>& layers() const;
        const std::vector<VkPhysicalDevice>& devices() const;

        void run();

    protected:
        Application(const Core::WindowProperties& windowConfig, VkPresentModeKHR presentMode, bool enableValidationLayers);

        Platform::Pointer::Scope<Core::Window> window;
        Platform::Pointer::Scope<Instance> instance;
    };
}

#endif