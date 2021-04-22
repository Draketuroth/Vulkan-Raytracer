
#include "Vulkan/ImageView.h"

#include "Vulkan/Device.h"

namespace Vulkan 
{
    ImageView::ImageView(const class Device& deviceIn, VkImage imageIn, VkFormat formatIn, VkImageAspectFlags aspectFlags) :
        device(deviceIn),
        image(imageIn),
        format(formatIn)
    {
        VkImageViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = imageIn;
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = format;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = aspectFlags;
        createInfo.subresourceRange.baseMipLevel = 0u;
        createInfo.subresourceRange.levelCount = 1u;
        createInfo.subresourceRange.baseArrayLayer = 0u;
        createInfo.subresourceRange.layerCount = 1u;

        CheckVKResult(vkCreateImageView(device.getHandle(), &createInfo, nullptr, &imageView), "Create image view");
    }

    ImageView::~ImageView()
    {
        if (imageView != nullptr) 
        {
            vkDestroyImageView(device.getHandle(), imageView, nullptr);
            imageView = nullptr;
        }
    }
}