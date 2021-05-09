
#include "Vulkan/Assets/UniformBuffer.h"

#include "Vulkan/Buffer.h"

namespace Vulkan
{
    namespace Assets
    {
        UniformBuffer::UniformBuffer(const Vulkan::Device& device)
        {
            const auto bufferSize = sizeof(UniformBufferObject);

            buffer.reset(new Vulkan::Buffer(device, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT));
            memory.reset(new Vulkan::DeviceMemory(buffer->allocateMemory(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)));
        }

        UniformBuffer::UniformBuffer(UniformBuffer&& other) noexcept :
            buffer(other.buffer.release()),
            memory(other.memory.release())
        {
        }

        UniformBuffer::~UniformBuffer()
        {
            buffer.reset();
            memory.reset(); // Release memory after bound buffer has been destroyed.
        }

        void UniformBuffer::setValue(const UniformBufferObject& ubo)
        {
            const auto data = memory->map(0, sizeof(UniformBufferObject));
            std::memcpy(data, &ubo, sizeof(ubo));
            memory->unmap();
        }
    }
}