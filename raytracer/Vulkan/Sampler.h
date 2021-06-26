#ifndef VULKAN_SAMPLER_H
#define VULKAN_SAMPLER_H

#include "Vulkan/Core.h"

namespace Vulkan { class Device; }

namespace Vulkan 
{
    struct SamplerConfig final
    {
        VkFilter magFilter = VK_FILTER_LINEAR;
        VkFilter minFilter = VK_FILTER_LINEAR;
        VkSamplerAddressMode addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        VkSamplerAddressMode addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        VkSamplerAddressMode addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        bool anistropyEnable = true;
        float maxAnistropy = 16.0f;
        VkBorderColor borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        bool unnormalizedCoordinates = false;
        bool compareEnable = false;
        VkCompareOp compareOp = VK_COMPARE_OP_ALWAYS;
        VkSamplerMipmapMode mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        float mipLodBias = 0.0f;
        float minLod = 0.0f;
        float maxLod = 0.0f;
    };

    class Sampler final 
    {
    public:

        Sampler(const Device& deviceIn, const SamplerConfig& config);
        ~Sampler();

        const class Device& getDevice() const { return device; }
        VkSampler getHandle() const { return sampler; }

    private:
        Sampler(const Sampler&) = delete;
        Sampler(Sampler&&) = delete;
        Sampler& operator = (const Sampler&) = delete;
        Sampler& operator = (Sampler&&) = delete;


        const class Device& device;
        VkSampler sampler;
    };
}

#endif