#ifndef VULKAN_IMAGE_H
#define VULKAN_IMAGE_H

#include "Vulkan/DeviceMemory.h"

namespace Vulkan { 
    class Device; 
    class CommandPool;
    class Buffer;
}

namespace Vulkan 
{
    class Image final 
    {
    public:

        Image(const Device& deviceIn, VkExtent2D extentIn, VkFormat formatIn);
        Image(const Device& deviceIn, VkExtent2D extentIn, VkFormat formatIn, VkImageTiling tiling, VkImageUsageFlags usage);
        Image(Image&& other) noexcept;
        ~Image();

        VkImage getHandle() const { return image; }
        const class Device& getDevice() const { return device; }
        VkExtent2D getExtent() const { return extent; }
        VkFormat getFormat() const { return format; }

        DeviceMemory allocateMemory(VkMemoryPropertyFlags properties) const;
        VkMemoryRequirements getMemoryRequirements() const;

        void transitionImageLayout(CommandPool& commandPool, VkImageLayout newLayout);
        void copyFrom(CommandPool& commandPool, const Buffer& buffer);

    private:

        const class Device& device;
        const VkExtent2D extent;
        const VkFormat format;
        VkImageLayout imageLayout;

        VkImage image;
    };
}

#endif