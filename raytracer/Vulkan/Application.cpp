
#include "Vulkan/Application.h"

#include "Vulkan/Debug/Messenger.h"
#include "Vulkan/Instance.h"
#include "Vulkan/Surface.h"
#include "Vulkan/Device.h"
#include "Vulkan/CommandPool.h"
#include "Vulkan/SwapChain.h"
#include "Vulkan/DepthBuffer.h"
#include "Vulkan/CommandBuffers.h"
#include "Vulkan/Resources/UniformBuffer.h"
#include "Vulkan/GraphicsPipeline.h"
#include "Vulkan/Semaphore.h"
#include "Vulkan/Fence.h"
#include "Vulkan/FrameBuffer.h"

#include "Core/Window.h"

namespace Vulkan 
{
    Application::~Application()
    {
        deleteSwapchain();

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

        createSwapChain();
    }

    void Application::createSwapChain()
    {
        // Only process events when the window is visible.
        while (window->isMinimized()) 
        {
            window->waitForEvents();
        }

        swapChain.reset(new SwapChain(*device, presentMode));
        depthBuffer.reset(new DepthBuffer(*commandPool, swapChain->getExtent()));

        for (size_t i = 0; i != swapChain->getImageViews().size(); ++i) 
        {
            imageAvailableSemaphores.emplace_back(*device);
            renderFinishedSemaphores.emplace_back(*device);
            inFlightFences.emplace_back(*device, true);
            uniformBuffers.emplace_back(*device);
        }

        graphicsPipeline.reset(new class GraphicsPipeline(*swapChain, *depthBuffer, uniformBuffers, getScene(), isWireFrame));

        for (const auto& imageView : swapChain->getImageViews()) 
        {
            swapChainFrameBuffers.emplace_back(*imageView, graphicsPipeline->getRenderPass());
        }

        commandBuffers.reset(new CommandBuffers(*commandPool, static_cast<Platform::Type::Uint32>(swapChainFrameBuffers.size())));
    }

    void Application::run()
    {
        window->run();
    }
    
    Application::Application(const Core::WindowProperties& windowProperties, VkPresentModeKHR presentModeIn, bool enableValidationLayers) :
        presentMode(presentModeIn)
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

    void Application::deleteSwapchain()
    {
        commandBuffers.reset();
        swapChainFrameBuffers.clear();
        graphicsPipeline.reset();
        uniformBuffers.clear();
        inFlightFences.clear();
        renderFinishedSemaphores.clear();
        imageAvailableSemaphores.clear();
        depthBuffer.reset();
        swapChain.reset();
    }
}