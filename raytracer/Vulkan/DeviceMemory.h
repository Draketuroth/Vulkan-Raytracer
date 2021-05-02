#ifndef VULKAN_DEVICE_MEMORY_H
#define VULKAN_DEVICE_MEMORY_H

namespace Vulkan { class Device; }

namespace Vulkan 
{
    class DeviceMemory final 
    {
    public:

        DeviceMemory(const Device& deviceIn, size_t size, 
            Platform::Type::Uint32 memoryTypeBits, 
            VkMemoryAllocateFlags allocateFlags, 
            VkMemoryPropertyFlags propertyFlags);

        DeviceMemory(DeviceMemory&& other) noexcept;
        ~DeviceMemory();


        VkDeviceMemory getHandle() const { return memory; }
        const class Device& getDevice() const { return device; }

        void* map(size_t offset, size_t size);
        void unmap();

    private:

        Platform::Type::Uint32 findMemoryType(Platform::Type::Uint32 typeFilter,
            const VkMemoryPropertyFlags propertyFlags) const;

        const class Device& device;

        VkDeviceMemory memory;
    };
}

#endif