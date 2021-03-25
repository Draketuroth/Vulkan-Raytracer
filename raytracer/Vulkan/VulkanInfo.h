#ifndef VULKAN_INFO_H
#define VULKAN_INFO_H

namespace Vulkan { class Application; }

namespace Vulkan 
{
    void PrintVulkanSDKInformation();
    void PrintVulkanInstanceInformation(const Application& application);
    void PrintVulkanLayersInformation(const Vulkan::Application& application);
    void PrintVulkanDevices(const Vulkan::Application& application);
    void PrintVulkanSwapChainInformation(const Vulkan::Application& application);
}

#endif
