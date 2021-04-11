
#include "Vulkan/Application.h"

#include "Vulkan/Debug/Messenger.h"
#include "Vulkan/Instance.h"
#include "Vulkan/Surface.h"
#include "Vulkan/Device.h"
#include "Vulkan/CommandPool.h"

#include "Core/Window.h"

namespace Vulkan 
{
    Application::~Application()
    {
        commandPool.reset();
        device.reset();
        surface.reset();
        debugUtilsMessenger.reset();
        instance.reset();
        window.reset();
    }

    const std::vector<VkExtensionProperties>& Application::extensions() const
    {
        return instance->getExtensions();
    }

    const std::vector<VkLayerProperties>& Application::layers() const
    {
        return instance->getLayers();
    }

    const std::vector<VkPhysicalDevice>& Application::devices() const
    {
        return instance->getDevices();
    }

    void Application::setPhysicalDevice(VkPhysicalDevice physicalDevice)
    {
        if (device) 
        {
            throw std::logic_error("Physical device has already been set");
        }

        std::vector<const Platform::Type::Char*> requiredExtensions =
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        VkPhysicalDeviceFeatures deviceFeatures = {};

        setPhysicalDevice(physicalDevice, requiredExtensions, deviceFeatures, nullptr);

    }

    void Application::run()
    {
        window->run();
    }
    
    Application::Application(const Core::WindowProperties& windowProperties, VkPresentModeKHR presentMode, bool enableValidationLayers)
    {
        const std::vector<const Platform::Type::Char*> validationLayers = enableValidationLayers
            ? std::vector<const Platform::Type::Char*> {"VK_LAYER_KHRONOS_validation"} : std::vector<const Platform::Type::Char*>();

        window.reset(new Core::Window(windowProperties));
        instance.reset(new Instance(*window, validationLayers, VK_API_VERSION_1_2));
        debugUtilsMessenger.reset(enableValidationLayers ? new Debug::Messenger(*instance, VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) : nullptr);
        surface.reset(new Surface(*instance));
    }

    void Application::setPhysicalDevice(VkPhysicalDevice physicalDevice, std::vector<const Platform::Type::Char*>& requiredExtensions, VkPhysicalDeviceFeatures& deviceFeatures, void* nextDeviceFeatures)
    {
        device.reset(new class Device(physicalDevice, *surface, requiredExtensions, deviceFeatures, nextDeviceFeatures));
        commandPool.reset(new class CommandPool(*device, device->getGraphicsFamiliyIndex(), true));
    }
}