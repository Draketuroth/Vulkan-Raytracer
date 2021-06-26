#ifndef VULKAN_BUFFER_UTILITIES_H
#define VULKAN_BUFFER_UTILITIES_H

#include "Platform/StandardTypes.h"
#include "Vulkan/Core.h"
#include "Vulkan/Buffer.h"
#include "Vulkan/CommandPool.h"
#include "Vulkan/Device.h"
#include "Vulkan/DeviceMemory.h"

#include <vector>
#include <memory>
#include <string>
#include <cstring>

namespace Vulkan 
{
    class BufferUtilities final
    {
    public:

        template <class T>
        static void CopyFromStagingBuffer(CommandPool& commandPool, Buffer& dstBuffer, const std::vector<T> content);

        template <class T>
        static void CreateDeviceBuffer(
            CommandPool& commandPool,
            const Platform::Type::Char* name,
            VkBufferUsageFlags usage,
            const std::vector<T>& content,
            Platform::Pointer::Scope<Buffer>& buffer,
            Platform::Pointer::Scope<DeviceMemory>& memory);
    };

    template <class T>
    void BufferUtilities::CopyFromStagingBuffer(CommandPool& commandPool, Buffer& dstBuffer, const std::vector<T> content) 
    {
        const auto& device = commandPool.getDevice();
        const auto& contentSize = sizeof(content[0]) * content.size();

        // Create a temporary host-visible staging buffer.
        auto stagingBuffer = std::make_unique<Buffer>(device, contentSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
        auto stagingBufferMemory = stagingBuffer->allocateMemory(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        // Copy the host data into the staging buffer.
        const auto data = stagingBufferMemory.map(0, contentSize);
        std::memcpy(data, content.data(), contentSize);
        stagingBufferMemory.unmap();

        // Copy the staging buffer to the device buffer.
        dstBuffer.copyFrom(commandPool, *stagingBuffer, contentSize);

        // Delete the buffer.
        stagingBuffer.reset();
    }

    template <class T>
    void BufferUtilities::CreateDeviceBuffer(
        CommandPool& commandPool,
        const Platform::Type::Char* name,
        VkBufferUsageFlags usage,
        const std::vector<T>& content,
        Platform::Pointer::Scope<Buffer>& buffer,
        Platform::Pointer::Scope<DeviceMemory>& memory) 
    {
        const auto& device = commandPool.getDevice();
        const auto& debugUtils = device.getDebugUtilities();
        const auto contentSize = sizeof(content[0]) * content.size();
        const VkMemoryAllocateFlags allocateFlags = usage & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
            ? VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT 
            : 0u;

        buffer.reset(new Buffer(device, contentSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage));
        memory.reset(new DeviceMemory(buffer->allocateMemory(allocateFlags, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)));

        debugUtils.setObjectName(buffer->getHandle(), (name + std::string(" Buffer")).c_str());
        debugUtils.setObjectName(memory->getHandle(), (name + std::string(" Memory")).c_str());
        
        CopyFromStagingBuffer(commandPool, *buffer, content);
    }
}

#endif