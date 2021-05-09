#ifndef VULKAN_APPLICATION_H
#define VULKAN_APPLICATION_H

#include "Core/WindowProperties.h"

namespace Core { class Window; }

namespace Vulkan { 

    namespace Debug 
    { 
        class Messenger; 
    } 

    namespace Assets
    {
        class UniformBufferObject;
        class UniformBuffer;
    }
}

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
        void createSwapChain();

        void run();

    protected:
        Application(const Core::WindowProperties& windowConfig, VkPresentModeKHR presentModeIn, bool enableValidationLayers);

        virtual void setPhysicalDevice(
            VkPhysicalDevice physicalDevice,
            std::vector<const Platform::Type::Char*>& requiredExtensions,
            VkPhysicalDeviceFeatures& deviceFeatures,
            void* nextDeviceFeatures);

        virtual void deleteSwapchain();

    private:

        const VkPresentModeKHR presentMode;

        Platform::Pointer::Scope<Core::Window> window;
        Platform::Pointer::Scope<Debug::Messenger> debugUtilsMessenger;

        Platform::Pointer::Scope<class Device> device;
        Platform::Pointer::Scope<class CommandPool> commandPool;
        Platform::Pointer::Scope<class Instance> instance;
        Platform::Pointer::Scope<class Surface> surface;
        Platform::Pointer::Scope<class SwapChain> swapChain;
        Platform::Pointer::Scope<class DepthBuffer> depthBuffer;
        Platform::Pointer::Scope<class CommandBuffers> commandBuffers;
        
        std::vector<Assets::UniformBuffer> uniformBuffers;

        std::vector<class Semaphore> imageAvailableSemaphores;
        std::vector<class Semaphore> renderFinishedSemaphores;
        std::vector<class Fence> inFlightFences;



    };
}

#endif