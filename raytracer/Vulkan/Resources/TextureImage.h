#ifndef VULKAN_TEXTURE_IMAGE_H
#define VULKAN_TEXTURE_IMAGE_H

namespace Vulkan 
{
    class CommandPool;
    class DeviceMemory;
    class Image;
    class ImageView;
    class Sampler;
    
}

namespace Vulkan 
{
    namespace Resources 
    {
        class Texture;
    }
}

namespace Vulkan 
{
    namespace Resources 
    {
        class TextureImage final 
        {
        public:

            TextureImage(Vulkan::CommandPool& commandPool, const Texture& texture);
            ~TextureImage();

            const Vulkan::ImageView& getImageView() const { return *imageView; }
            const Vulkan::Sampler& getSampler() const { return *sampler; }

        private:
            TextureImage(const TextureImage&) = delete;
            TextureImage(TextureImage&&) = delete;
            TextureImage& operator = (const TextureImage&) = delete;
            TextureImage& operator = (TextureImage&&) = delete;

            Platform::Pointer::Scope<Vulkan::Image> image;
            Platform::Pointer::Scope<Vulkan::DeviceMemory> imageMemory;
            Platform::Pointer::Scope<Vulkan::ImageView> imageView;
            Platform::Pointer::Scope<Vulkan::Sampler> sampler;
        };
    }
}

#endif