
#include "Vulkan/VulkanInfo.h"

#include "Vulkan/Application.h"
#include "Vulkan/Version.h"

enum VendorId
{
    AMD = 0x1002,
    ImgTec = 0x1010,
    Nvidia = 0x10DE,
    ARM = 0x13B5,
    Qualcomm = 0x5143,
    Intel = 0x8086
};

namespace Vulkan 
{
    const char* DeviceType(const VkPhysicalDeviceType deviceType)
    {
        switch (deviceType)
        {
#define STR(value) case VkPhysicalDeviceType:: ##value: return #value
            STR(VK_PHYSICAL_DEVICE_TYPE_OTHER);
            STR(VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU);
            STR(VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);
            STR(VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU);
            STR(VK_PHYSICAL_DEVICE_TYPE_CPU);
#undef STR
        default:
            return "Unknown Device Type!";
        }
    }

    const char* VendorID(const Platform::Type::Uint32 vendorId) 
    {
        switch (vendorId)
        {
#define STR(value) case VendorId:: ##value: return #value
            STR(AMD);
            STR(ImgTec);
            STR(Nvidia);
            STR(ARM);
            STR(Qualcomm);
            STR(Intel);
#undef STR
        default:
            "Unknown Vendor!";
        }
    }

    void PrintVulkanSDKInformation()
    {
        std::cout << "Vulkan SDK Header Version: " << VK_HEADER_VERSION << std::endl;
        std::cout << std::endl;
    }

    void PrintVulkanInstanceInformation(const Application& application)
    {
        std::cout << "Vulkan Instance Extensions: " << std::endl;

        for (const auto& extension : application.extensions()) 
        {
            std::cout << "- " << extension.extensionName << " (" << Version(extension.specVersion) << ")" << std::endl;
        }

        std::cout << std::endl;
    }

    void PrintVulkanLayersInformation(const Vulkan::Application& application)
    {
        std::cout << "Vulkan Instance Layers: " << std::endl;

        for (const auto& layer : application.layers()) 
        {
            std::cout
                << "- " << layer.layerName
                << " (" << Version(layer.specVersion) << ")"
                << " : " << layer.description << std::endl;
        }

        std::cout << std::endl;
    }

    void PrintVulkanDevices(const Vulkan::Application& application)
    {
        std::cout << "Vulkan Devices: " << std::endl;

        for (const auto& device : application.devices()) 
        {
            VkPhysicalDeviceDriverProperties driverProperties{};
            driverProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRIVER_PROPERTIES;

            VkPhysicalDeviceProperties2 deviceProperties{};
            deviceProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
            deviceProperties.pNext = &driverProperties;

            vkGetPhysicalDeviceProperties2(device, &deviceProperties);

            VkPhysicalDeviceFeatures features;
            vkGetPhysicalDeviceFeatures(device, &features);

            const auto& property = deviceProperties.properties;

            const Version vulkanVersion(property.apiVersion);
            const Version driverVersion(property.driverVersion, property.vendorID);

            std::cout << "- [" << property.deviceID << "] ";
            std::cout << VendorID(property.vendorID) << " '" << property.deviceName;
            std::cout << "' (";
            std::cout << DeviceType(property.deviceType) << ": ";
            std::cout << "vulkan " << vulkanVersion << ", ";
            std::cout << "driver " << driverProperties.driverName << " " << driverProperties.driverInfo << " - " << driverVersion;
            std::cout << ")" << std::endl;
        }

        std::cout << std::endl;
    }

    void PrintVulkanSwapChainInformation(const Vulkan::Application& application)
    {

    }
}