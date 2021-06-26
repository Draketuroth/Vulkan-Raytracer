
#include "Vulkan/Resources/Texture.h"

#include "Utilities/StbImage.h"
#include <chrono>

namespace Vulkan 
{
    namespace Resources 
    {
        Texture Texture::loadTexture(const std::string& filename, const Vulkan::SamplerConfig& samplerConfig)
        {
            std::cout << "- loading '" << filename << "'... " << std::flush;
            const auto timer = std::chrono::high_resolution_clock::now();

            // Load the texture in normal host memory.
            Platform::Type::Int width, height, channels;
            const auto pixels = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);

            if (!pixels) 
            {
                throw std::runtime_error("Failed to load texture image '" + filename + "'");
            }

            const auto elapsed = std::chrono::duration<float, std::chrono::seconds::period>(std::chrono::high_resolution_clock::now() - timer).count();
            std::cout << "(" << width << " x " << height << " x " << channels << ") ";
            std::cout << elapsed << "s" << std::endl;

            return Texture(width, height, channels, pixels);
        }

        Texture::Texture(Platform::Type::Int widthIn, Platform::Type::Int heightIn, Platform::Type::Int channelsIn, Platform::Type::UChar* pixelsIn) :
            width(widthIn),
            height(heightIn),
            channels(channelsIn),
            pixels(pixelsIn, stbi_image_free)
        {

        }
    }
}