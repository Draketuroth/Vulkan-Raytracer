
#include "Vulkan/Resources/TextureImage.h"
#include "Vulkan/Resources/Texture.h"
#include "Vulkan/Buffer.h"
#include "Vulkan/CommandPool.h"
#include "Vulkan/ImageView.h"
#include "Vulkan/Image.h"
#include "Vulkan/Sampler.h"
#include "Vulkan/DeviceMemory.h"

namespace Vulkan
{
    namespace Resources
    {
        TextureImage::TextureImage(Vulkan::CommandPool& commandPool, const Texture& texture)
        {
            // Create a host staging buffer and copy the image into it.
            const VkDeviceSize imageSize = texture.getWidth() * texture.getHeight() * 4;
            const Device& device = commandPool.getDevice();

            auto stagingBuffer = std::make_unique<Vulkan::Buffer>(device, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
            auto stagingBufferMemory = stagingBuffer->allocateMemory(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

            const auto data = stagingBufferMemory.map(0, imageSize);
            std::memcpy(data, texture.getPixels(), imageSize);
            stagingBufferMemory.unmap();

            // Create the device side image, memory, view and sampler.
            image.reset(new Vulkan::Image(device, VkExtent2D{ static_cast<Platform::Type::Uint32>(texture.getWidth()),static_cast<Platform::Type::Uint32>(texture.getHeight()) }, VK_FORMAT_R8G8B8A8_UNORM));
            imageMemory.reset(new Vulkan::DeviceMemory(image->allocateMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)));
            imageView.reset(new Vulkan::ImageView(device, image->getHandle(), image->getFormat(), VK_IMAGE_ASPECT_COLOR_BIT));
            sampler.reset(new Vulkan::Sampler(device, Vulkan::SamplerConfig()));

            // Transfer the data to device side.
            image->transitionImageLayout(commandPool, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
            image->copyFrom(commandPool, *stagingBuffer);
            image->transitionImageLayout(commandPool, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

            // Delete the buffer
            stagingBuffer.reset();
        }

        TextureImage::~TextureImage()
        {
            sampler.reset();
            imageView.reset();
            imageMemory.reset();
            image.reset();
        }
    }
}