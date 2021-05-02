#ifndef VULKAN_BUFFER_H
#define VULKAN_BUFFER_H

#include "DeviceMemory.h"

namespace Vulkan { 
    class Device; 
    class CommandPool; 
};

namespace Vulkan 
{
    class Buffer final 
    {
    public:

        Buffer(const Device& deviceIn, size_t size, VkBufferUsageFlags usage);
        ~Buffer();

        VkBuffer getHandle() const { return buffer; }
        const class Device& getDevice() const { return device; }

        DeviceMemory allocateMemory(VkMemoryPropertyFlags properyFlags);
        DeviceMemory allocateMemory(VkMemoryAllocateFlags allocateFlags, VkMemoryPropertyFlags propertyFlags);
        VkMemoryRequirements getMemoryRequirements() const;
        VkDeviceAddress getDeviceAddress() const;

        void copyFrom(CommandPool& commandPool, const Buffer& src, VkDeviceSize size);


    private:

        const class Device& device;
        VkBuffer buffer;
    };
}

#endif