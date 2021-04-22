#ifndef VULKAN_IMAGE_VIEW_H
#define VULKAN_IMAGE_VIEW_H

namespace Vulkan { class Device; }

namespace Vulkan 
{
    class ImageView final 
    {
    public:

        explicit ImageView(const Device& deviceIn, VkImage imageIn, VkFormat formatIn, VkImageAspectFlags aspectFlags);
        ~ImageView();

        VkImageView getHandle() const { return imageView; }
        const class Device& getDevice() const { return device; }

    private:

        const class Device& device;
        const VkImage image;
        const VkFormat format;

        VkImageView imageView;
    };
}

#endif