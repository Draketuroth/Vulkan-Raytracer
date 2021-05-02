
#include "Vulkan/DepthBuffer.h"

#include "Vulkan/Device.h"
#include "Vulkan/CommandPool.h"
#include "Vulkan/DeviceMemory.h"
#include "Vulkan/Image.h"
#include "Vulkan/ImageView.h"

namespace Vulkan 
{
    namespace 
    {
        VkFormat findSupportedFormat(
            const Device& device, 
            const std::vector<VkFormat>& candidates, 
            const VkImageTiling tiling, 
            const VkFormatFeatureFlags features) 
        {
            for (VkFormat format : candidates) 
            {
                VkFormatProperties formatProperties;
                vkGetPhysicalDeviceFormatProperties(device.getPhysicalDevice(), format, &formatProperties);

                if (tiling == VK_IMAGE_TILING_LINEAR && (formatProperties.linearTilingFeatures & features) == features) 
                {
                    return format;
                }

                if (tiling == VK_IMAGE_TILING_OPTIMAL && (formatProperties.optimalTilingFeatures & features) == features) 
                {
                    return format;
                }
            }

            throw std::runtime_error("Failed to find supported format!");
        }

        VkFormat findDepthFormat(const Device& device) 
        {
            return findSupportedFormat(
                device,
                { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
                VK_IMAGE_TILING_OPTIMAL,
                VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
            );
        }
    }

    DepthBuffer::DepthBuffer(CommandPool& commandPool, VkExtent2D extent) :
        format(findDepthFormat(commandPool.getDevice()))
    {
        const Device& device = commandPool.getDevice();

        image.reset(new Image(device, extent, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT));
        imageMemory.reset(new DeviceMemory(image->allocateMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)));
        imageView.reset(new ImageView(device, image->getHandle(), format, VK_IMAGE_ASPECT_DEPTH_BIT));

        image->transitionImageLayout(commandPool, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

        const auto& debugUtilities = device.getDebugUtilities();
        debugUtilities.setObjectName(image->getHandle(), "Depth Buffer Image");
        debugUtilities.setObjectName(imageMemory->getHandle(), "Depth Buffer Image Memory");
        debugUtilities.setObjectName(imageView->getHandle(), "Depth Buffer ImageView");
    }

    DepthBuffer::~DepthBuffer()
    {
        imageView.reset();
        image.reset();
        imageMemory.reset(); // Release memory after bound image has been destroyed.
    }
}