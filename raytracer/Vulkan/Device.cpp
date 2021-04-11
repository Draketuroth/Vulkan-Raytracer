
#include "Vulkan/Device.h"

#include "Vulkan/Enumerate.h"
#include "Vulkan/Surface.h"
#include "Vulkan/Instance.h"

#include <set>

namespace Vulkan 
{
    namespace
    {
        std::vector<VkQueueFamilyProperties>::const_iterator FindQueue(
            const std::vector<VkQueueFamilyProperties>& queueFamilies,
            const std::string& name, 
            const VkQueueFlags requiredBits,
            const VkQueueFlags excludedBits) 
        {
            const auto family = std::find_if(queueFamilies.begin(), queueFamilies.end(), [requiredBits, excludedBits](const VkQueueFamilyProperties& queueFamily) 
            {
                    return
                        queueFamily.queueCount > 0 &&
                        queueFamily.queueFlags & requiredBits &&
                        !(queueFamily.queueFlags & excludedBits);
            });

            if (family == queueFamilies.end()) 
            {
                throw std::runtime_error("Could not find matching " + name + " queue");
            }

            return family;
        }
    }


    Device::Device(
        VkPhysicalDevice physicalDevice, 
        const Surface& surface, 
        const std::vector<const Platform::Type::Char*>& requiredExtensions,
        const VkPhysicalDeviceFeatures& deviceFeatures, 
        const void* nextDeviceFeatures) :
        physicalDevice(physicalDevice),
        surface(surface),
        debugUtilites(surface.getInstance().getHandle())
    {
        checkRequiredExtensions(physicalDevice, requiredExtensions);

        const auto queueFamilies = GetEnumerateVector(physicalDevice, vkGetPhysicalDeviceQueueFamilyProperties);

        // Locate the graphics queue.
        const auto graphicsFamily = FindQueue(queueFamilies, "graphics", VK_QUEUE_GRAPHICS_BIT, 0);
        const auto computeFamily = FindQueue(queueFamilies, "compute", VK_QUEUE_COMPUTE_BIT, VK_QUEUE_GRAPHICS_BIT);
        const auto transferFamily = FindQueue(queueFamilies, "transfer", VK_QUEUE_TRANSFER_BIT, VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);

        // Locate the presentation queue, which usually is the same as the graphics queue.
        const auto presentFamily = std::find_if(queueFamilies.begin(), queueFamilies.end(), [&](const VkQueueFamilyProperties& queueFamily)
        {
            VkBool32 presentSupport = false;
            const Platform::Type::Uint32 i = static_cast<Platform::Type::Uint32>(&*queueFamilies.cbegin() - &queueFamily);
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface.getHandle(), &presentSupport);
            return queueFamily.queueCount > 0 && presentSupport;
        });

        if (presentFamily == queueFamilies.end()) 
        {
            throw std::runtime_error("Could not find any presentation queue!");
        }

        graphicsFamilyIndex = static_cast<Platform::Type::Uint32>(graphicsFamily - queueFamilies.begin());
        computeFamilyIndex = static_cast<Platform::Type::Uint32>(computeFamily - queueFamilies.begin());
        presentFamilyIndex = static_cast<Platform::Type::Uint32>(presentFamily - queueFamilies.begin());
        transferFamilyIndex = static_cast<Platform::Type::Uint32>(transferFamily - queueFamilies.begin());

        // Queues can be the same.
        const std::set<Platform::Type::Uint32> uniqueQueueFamilies =
        {
            graphicsFamilyIndex,
            computeFamilyIndex,
            presentFamilyIndex,
            transferFamilyIndex
        };

        // Create queues.
        Platform::Type::Float queuePriority = 1.0f;
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

        for (Platform::Type::Uint32 queueFamilyIndex : uniqueQueueFamilies) 
        {
            VkDeviceQueueCreateInfo queueCreateInfo = {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;

            queueCreateInfos.push_back(queueCreateInfo);
        }

        // Create the device.
        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pNext = nextDeviceFeatures;
        createInfo.queueCreateInfoCount = static_cast<Platform::Type::Uint32>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledLayerCount = static_cast<Platform::Type::Uint32>(surface.getInstance().getValidationLayers().size());
        createInfo.ppEnabledLayerNames = surface.getInstance().getValidationLayers().data();
        createInfo.enabledExtensionCount = static_cast<Platform::Type::Uint32>(requiredExtensions.size());
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();

        CheckVKResult(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device), "Create logical device");

        debugUtilites.setDevice(device);

        vkGetDeviceQueue(device, graphicsFamilyIndex, 0, &graphicsQueue);
        vkGetDeviceQueue(device, computeFamilyIndex, 0, &computeQueue);
        vkGetDeviceQueue(device, presentFamilyIndex, 0, &presentQueue);
        vkGetDeviceQueue(device, transferFamilyIndex, 0, &transferQueue);
    }

    Device::~Device()
    {
        if (device != nullptr) 
        {
            vkDestroyDevice(device, nullptr);
            device = nullptr;
        }
    }

    void Device::waitIdle() const
    {
        CheckVKResult(vkDeviceWaitIdle(device), "Wait for device idle");
    }

    void Device::checkRequiredExtensions(VkPhysicalDevice physicalDevice, const std::vector<const Platform::Type::Char*>& requiredExtensions) const
    {
        const auto availableExtensions = GetEnumerateVector(physicalDevice, static_cast<const char*>(nullptr), vkEnumerateDeviceExtensionProperties);
        std::set<std::string> required(requiredExtensions.begin(), requiredExtensions.end());

        for (const auto& extension : availableExtensions) 
        {
            required.erase(extension.extensionName);
        }

        if (!required.empty()) 
        {
            bool first = true;
            std::string extensions;

            for (const auto& extension : required) 
            {
                if (!first) 
                {
                    extensions += ", ";
                }

                extensions += extension;
                first = false;
            }

            throw std::runtime_error("Missing required extensions: " + extensions);
        }
    }
}