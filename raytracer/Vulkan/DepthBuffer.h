#ifndef VULKAN_DEPTH_BUFFER_H
#define VULKAN_DEPTH_BUFFER_H

namespace Vulkan  
{ 
    class CommandPool;
}

namespace Vulkan 
{
    class DepthBuffer final 
    {
    public:
        DepthBuffer(CommandPool& commandPool, VkExtent2D extent);
        ~DepthBuffer();

        VkFormat getFormat() const { return format; }
        const class ImageView& getImageView() const { return *imageView; }

        static bool hasStencilComponent(const VkFormat format) 
        {
            return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
        }

    private:

        const VkFormat format;
        Platform::Pointer::Scope<class Image> image;
        Platform::Pointer::Scope<class DeviceMemory> imageMemory;
        Platform::Pointer::Scope<class ImageView> imageView;
    };
}

#endif