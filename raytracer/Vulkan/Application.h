#ifndef VULKAN_APPLICATION_H
#define VULKAN_APPLICATION_H

#include "Core/WindowProperties.h"

namespace Core { class Window; }

namespace Vulkan { namespace Debug { class Messenger; } }

namespace Vulkan 
{
    class Application 
    {
    public:
        virtual ~Application();

        const std::vector<VkExtensionProperties>& extensions() const;
        const std::vector<VkLayerProperties>& layers() const;
        const std::vector<VkPhysicalDevice>& devices() const;

        void setPhysicalDevice(VkPhysicalDevice physicalDevice);

        void run();

    protected:
        Application(const Core::WindowProperties& windowConfig, VkPresentModeKHR presentMode, bool enableValidationLayers);

        virtual void setPhysicalDevice(
            VkPhysicalDevice physicalDevice,
            std::vector<const Platform::Type::Char*>& requiredExtensions,
            VkPhysicalDeviceFeatures& deviceFeatures,
            void* nextDeviceFeatures);


        Platform::Pointer::Scope<Core::Window> window;
        Platform::Pointer::Scope<class Device> device;
        Platform::Pointer::Scope<class Instance> instance;
        Platform::Pointer::Scope<Debug::Messenger> debugUtilsMessenger;
        Platform::Pointer::Scope<class Surface> surface;
    };
}

#endif