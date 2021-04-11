#ifndef VULKAN_DEBUG_INFO_H
#define VULKAN_DEBUG_INFO_H

namespace Vulkan { class Application; }

namespace Vulkan 
{
    namespace Debug 
    {
        void PrintVulkanSDKInformation();
        void PrintVulkanInstanceInformation(const Application& application);
        void PrintVulkanLayersInformation(const Vulkan::Application& application);
        void PrintVulkanDevices(const Vulkan::Application& application);
        void PrintVulkanSwapChainInformation(const Vulkan::Application& application);
    }
}

#endif
