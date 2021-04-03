
#include "Vulkan/Device.h"

namespace Vulkan 
{
    Device::Device(
        VkPhysicalDevice physicalDevice, 
        const Surface& surface, 
        const std::vector<const Platform::Type::Char*>& requiredExtensions,
        const VkPhysicalDeviceFeatures& deviceFeatures, 
        const void* nextDeviceFeatures) :
        physicalDevice(physicalDevice),
        surface(surface)
    {

    }

    Device::~Device()
    {

    }

    void Device::waitIdle() const
    {

    }

    void Device::checkRequiredExtensions(VkPhysicalDevice physicalDevice, const std::vector<const Platform::Type::Char*>& requiredExtensions) const
    {

    }
}