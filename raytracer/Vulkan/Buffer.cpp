
#include "Vulkan/Buffer.h"

#include "Vulkan/Device.h"
#include "Vulkan/SingleTimeCommands.h"

namespace Vulkan 
{
    Buffer::Buffer(const Device& deviceIn, size_t size, VkBufferUsageFlags usage):
        device(deviceIn)
    {
        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        CheckVKResult(vkCreateBuffer(device.getHandle(), &bufferInfo, nullptr, &buffer), "Create buffer");
    }

    Buffer::~Buffer()
    {
        if (buffer != nullptr) 
        {
            vkDestroyBuffer(device.getHandle(), buffer, nullptr);
            buffer = nullptr;
        }
    }

    DeviceMemory Buffer::allocateMemory(VkMemoryPropertyFlags properyFlags)
    {
        return allocateMemory(0, properyFlags);
    }

    DeviceMemory Buffer::allocateMemory(VkMemoryAllocateFlags allocateFlags, VkMemoryPropertyFlags propertyFlags)
    {
        const VkMemoryRequirements reguirements = getMemoryRequirements();
        DeviceMemory memory(device, reguirements.size, reguirements.memoryTypeBits, allocateFlags, propertyFlags);

        CheckVKResult(vkBindBufferMemory(device.getHandle(), buffer, memory.getHandle(), 0), "Bind buffer memory");

        return memory;
    }

    VkMemoryRequirements Buffer::getMemoryRequirements() const
    {
        VkMemoryRequirements requirements;
        vkGetBufferMemoryRequirements(device.getHandle(), buffer, &requirements);
        return requirements;
    }

    VkDeviceAddress Buffer::getDeviceAddress() const
    {
        VkBufferDeviceAddressInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
        info.pNext = nullptr;
        info.buffer = buffer;

        return vkGetBufferDeviceAddress(device.getHandle(), &info);
    }

    void Buffer::copyFrom(CommandPool& commandPool, const Buffer& src, VkDeviceSize size)
    {
        SingleTimeCommands::submit(commandPool, [&](VkCommandBuffer commandBuffer)
        {
            VkBufferCopy copyRegion = {};
            copyRegion.srcOffset = 0;
            copyRegion.dstOffset = 0;
            copyRegion.size = size;

            vkCmdCopyBuffer(commandBuffer, src.getHandle(), getHandle(), 1u, &copyRegion);
        });
    }
}