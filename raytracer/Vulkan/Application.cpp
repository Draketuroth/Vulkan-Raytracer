
#include "Vulkan/Application.h"

#include "Vulkan/DebugMessenger.h"
#include "Vulkan/Instance.h"
#include "Vulkan/Surface.h"

#include "Core/Window.h"

namespace Vulkan 
{
    Application::~Application()
    {
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
        // if(device)
    }

    void Application::run()
    {
        window->run();
    }
    
    Application::Application(const Core::WindowProperties& windowProperties, VkPresentModeKHR presentMode, bool enableValidationLayers)
    {
        const std::vector<const char*> validationLayers = enableValidationLayers
            ? std::vector<const char*> {"VK_LAYER_KHRONOS_validation"} : std::vector<const char*>();

        window.reset(new Core::Window(windowProperties));
        instance.reset(new Instance(*window, validationLayers, VK_API_VERSION_1_2));
        debugUtilsMessenger.reset(enableValidationLayers ? new DebugMessenger(*instance, VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) : nullptr);
        surface.reset(new Surface(*instance));
    }
}