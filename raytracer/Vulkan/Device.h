#ifndef VULKAN_DEVICE_H
#define VULKAN_DEVICE_H

#include "Vulkan/Debug/Utilities.h"

namespace Vulkan {
    class Surface;
}

namespace Vulkan 
{
    class Device final 
    {
    public:
        Device(VkPhysicalDevice physicalDevice,
            const Surface& surface,
            const std::vector<const Platform::Type::Char*>& requiredExtensions,
            const VkPhysicalDeviceFeatures& deviceFeatures,
            const void* nextDeviceFeatures);

        ~Device();

        VkDevice getHandle() const { return device; }
        VkPhysicalDevice getPhysicalDevice() const { return physicalDevice; }
        const class Surface& getSurface() const { return surface; }

        const class Debug::Utilities& getDebugUtilities() const { return debugUtilites; }

        Platform::Type::Uint32 getGraphicsFamiliyIndex() const { return graphicsFamilyIndex; }
        Platform::Type::Uint32 getComputesFamiliyIndex() const { return computeFamilyIndex; }
        Platform::Type::Uint32 getPresentFamiliyIndex() const { return presentFamilyIndex; }
        Platform::Type::Uint32 getTransferFamiliyIndex() const { return transferFamilyIndex; }

        VkQueue getGraphicsQueue() const { return graphicsQueue; }
        VkQueue getComputeQueue() const { return computeQueue; }
        VkQueue getPresentQueue() const { return presentQueue; }
        VkQueue getTransferQueue() const { return transferQueue; }

        void waitIdle() const;

    private:

        void checkRequiredExtensions(VkPhysicalDevice physicalDevice, const std::vector<const Platform::Type::Char*>& requiredExtensions) const;

        const VkPhysicalDevice physicalDevice;
        const class Surface& surface;

        VkDevice device;

        class Debug::Utilities debugUtilites;

        Platform::Type::Uint32 graphicsFamilyIndex {};
        Platform::Type::Uint32 computeFamilyIndex{};
        Platform::Type::Uint32 presentFamilyIndex{};
        Platform::Type::Uint32 transferFamilyIndex{};

        VkQueue graphicsQueue{};
        VkQueue computeQueue{};
        VkQueue presentQueue{};
        VkQueue transferQueue{};
    };
}

#endif