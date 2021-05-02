
#include "Vulkan/DeviceMemory.h"

#include "Vulkan/Device.h"

namespace Vulkan
{
    DeviceMemory::DeviceMemory(const Device& deviceIn, size_t size, 
        Platform::Type::Uint32 memoryTypeBits, 
        VkMemoryAllocateFlags allocateFlags, 
        VkMemoryPropertyFlags propertyFlags) : 
        device(deviceIn)
    {
        VkMemoryAllocateFlagsInfo memoryInfo = {};
        memoryInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO;
        memoryInfo.pNext = nullptr;
        memoryInfo.flags = allocateFlags;

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.pNext = &memoryInfo;
        allocInfo.allocationSize = size;
        allocInfo.memoryTypeIndex = findMemoryType(memoryTypeBits, propertyFlags);

        CheckVKResult(vkAllocateMemory(device.getHandle(), &allocInfo, nullptr, &memory), "Allocate memory");
    }

    DeviceMemory::DeviceMemory(DeviceMemory&& other) noexcept :
        device(other.device),
        memory(other.memory) 
    {
        other.memory = nullptr;
    }

    DeviceMemory::~DeviceMemory()
    {
        if (memory != nullptr) 
        {
            vkFreeMemory(device.getHandle(), memory, nullptr);
            memory = nullptr;
        }
    }

    void* DeviceMemory::map(size_t offset, size_t size)
    {
        void* data = nullptr;
        CheckVKResult(vkMapMemory(device.getHandle(), memory, offset, size, 0, &data), "Map memory");

        return data;
    }

    void DeviceMemory::unmap()
    {
        vkUnmapMemory(device.getHandle(), memory);
    }

    Platform::Type::Uint32 DeviceMemory::findMemoryType(Platform::Type::Uint32 typeFilter, const VkMemoryPropertyFlags propertyFlags) const
    {
        VkPhysicalDeviceMemoryProperties memoryProperties;
        vkGetPhysicalDeviceMemoryProperties(device.getPhysicalDevice(), &memoryProperties);

        for (Platform::Type::Uint32 i = 0u; i != memoryProperties.memoryTypeCount; ++i) 
        {
            if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & propertyFlags) == propertyFlags) 
            {
                return i;
            }
        }

        throw std::runtime_error("Failed to find suitable memory type!");
    }
}