
#include "Vulkan/Image.h"

#include "Vulkan/Buffer.h"
#include "Vulkan/Device.h"
#include "Vulkan/DeviceMemory.h"
#include "Vulkan/SingleTimeCommands.h"
#include "Vulkan/DepthBuffer.h"

namespace Vulkan 
{
    Image::Image(const Device& deviceIn, VkExtent2D extentIn, VkFormat formatIn) :
        Image(deviceIn, extentIn, formatIn, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT)
    {
    }

    Image::Image(const Device& deviceIn, VkExtent2D extentIn, VkFormat formatIn, VkImageTiling tiling, VkImageUsageFlags usage) :
        device(deviceIn), extent(extentIn), format(formatIn), imageLayout(VK_IMAGE_LAYOUT_UNDEFINED)
    {
        VkImageCreateInfo imageInfo = {};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = extent.width;
        imageInfo.extent.height = extent.height;
        imageInfo.extent.depth = 1u;
        imageInfo.mipLevels = 1u;
        imageInfo.arrayLayers = 1u;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = imageLayout;
        imageInfo.usage = usage;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.flags = 0;

        CheckVKResult(vkCreateImage(device.getHandle(), &imageInfo, nullptr, &image), "Create image");
    }

    Image::Image(Image&& other) noexcept :
        device(other.device),
        extent(other.extent),
        format(other.format),
        imageLayout(other.imageLayout),
        image(other.image)
    {
        other.image = nullptr;
    }

    Image::~Image()
    {
        if (image != nullptr) 
        {
            vkDestroyImage(device.getHandle(), image, nullptr);
            image = nullptr;
        }
    }

    DeviceMemory Image::allocateMemory(VkMemoryPropertyFlags properties) const
    {
        VkMemoryRequirements requirements = getMemoryRequirements();
        DeviceMemory memory(device, requirements.size, requirements.memoryTypeBits, 0, properties);

        CheckVKResult(vkBindImageMemory(device.getHandle(), image, memory.getHandle(), 0), "Bind image memory");

        return memory;
    }

    VkMemoryRequirements Image::getMemoryRequirements() const
    {
        VkMemoryRequirements requirements;
        vkGetImageMemoryRequirements(device.getHandle(), image, &requirements);
        return requirements;
    }

    void Image::transitionImageLayout(CommandPool& commandPool, VkImageLayout newLayout)
    {
        SingleTimeCommands::submit(commandPool, [&](VkCommandBuffer commandBuffer)
        {
            VkImageMemoryBarrier barrier = {};
            barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier.oldLayout = imageLayout;
            barrier.newLayout = newLayout;
            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.image = image;
            barrier.subresourceRange.baseMipLevel = 0u;
            barrier.subresourceRange.levelCount = 1u;
            barrier.subresourceRange.baseArrayLayer = 0u;
            barrier.subresourceRange.layerCount = 1u;

            if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) 
            {
                barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

                if(DepthBuffer::hasStencilComponent(format))
                {
                    barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
                }
            }
            else 
            {
                barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            }

            VkPipelineStageFlags sourceStage;
            VkPipelineStageFlags destinationStage;

            if (imageLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) 
            {
                barrier.srcAccessMask = 0;
                barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

                sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            }
            else if (imageLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
            {
                barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

                sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            }
            else if (imageLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
            {
                barrier.srcAccessMask = 0;
                barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

                sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            }
            else 
            {
                throw std::invalid_argument("Unsupported layout transition!");
            }

            vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0u, nullptr, 0u, nullptr, 1u, &barrier);
        });
    }

    void Image::copyFrom(CommandPool& commandPool, const Buffer& buffer)
    {
        SingleTimeCommands::submit(commandPool, [&](VkCommandBuffer commandBuffer)
        {
                VkBufferImageCopy region = {};
                region.bufferOffset = 0;
                region.bufferRowLength = 0u;
                region.bufferImageHeight = 0u;
                region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                region.imageSubresource.mipLevel = 0u;
                region.imageSubresource.baseArrayLayer = 0u;
                region.imageSubresource.layerCount = 1u;
                region.imageOffset = { 0, 0, 0 };
                region.imageExtent = { extent.width, extent.height, 1 };

                vkCmdCopyBufferToImage(commandBuffer, buffer.getHandle(), image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1u, &region);
        });
    }
}