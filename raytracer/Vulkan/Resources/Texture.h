#ifndef VULKAN_TEXTURE_H
#define VULKAN_TEXTURE_H

#include "Vulkan/Sampler.h"

namespace Vulkan 
{
    namespace Resources 
    {
        class Texture final 
        {
        public:

            static Texture loadTexture(const std::string& filename, const Vulkan::SamplerConfig& samplerConfig);

            const Platform::Type::UChar* getPixels() const { return pixels.get(); }
            Platform::Type::Int getWidth() const { return width; }
            Platform::Type::Int getHeight() const { return height; }

            Texture() = default;
            Texture(const Texture&) = default;
            Texture(Texture&&) = default;
            ~Texture() = default;

        private:
            Texture& operator = (const Texture&) = delete;
            Texture& operator = (Texture&&) = delete;

            Texture(Platform::Type::Int widthIn, Platform::Type::Int heightIn, Platform::Type::Int channelsIn, Platform::Type::UChar* const pixelsIn);

            Vulkan::SamplerConfig samplerConfig;
            Platform::Type::Int width;
            Platform::Type::Int height;
            Platform::Type::Int channels;
            std::unique_ptr<unsigned char, void (*) (void*)> pixels;
        };
    }
}

#endif