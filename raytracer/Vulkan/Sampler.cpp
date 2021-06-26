
#include "Vulkan/Sampler.h"
#include "Vulkan/Device.h"

namespace Vulkan 
{
    Sampler::Sampler(const Device& deviceIn, const SamplerConfig& config) :
        device(deviceIn)
    {
        VkSamplerCreateInfo samplerInfo = {};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = config.magFilter;
        samplerInfo.minFilter = config.minFilter;
        samplerInfo.addressModeU = config.addressModeU;
        samplerInfo.addressModeV = config.addressModeV;
        samplerInfo.addressModeW = config.addressModeW;
        samplerInfo.anisotropyEnable = config.anistropyEnable;
        samplerInfo.maxAnisotropy = config.maxAnistropy;
        samplerInfo.borderColor = config.borderColor;
        samplerInfo.unnormalizedCoordinates = config.unnormalizedCoordinates;
        samplerInfo.compareEnable = config.compareEnable;
        samplerInfo.compareOp = config.compareOp;
        samplerInfo.mipmapMode = config.mipmapMode;
        samplerInfo.mipLodBias = config.mipLodBias;
        samplerInfo.minLod = config.minLod;
        samplerInfo.maxLod = config.maxLod;

        CheckVKResult(vkCreateSampler(device.getHandle(), &samplerInfo, nullptr, &sampler), "create sampler");
    }

    Sampler::~Sampler()
    {
        if (sampler != nullptr) 
        {
            vkDestroySampler(device.getHandle(), sampler, nullptr);
            sampler = nullptr;
        }
    }
}