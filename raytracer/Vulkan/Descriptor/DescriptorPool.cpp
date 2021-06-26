
#include "Vulkan/Descriptor/DescriptorPool.h"
#include "Vulkan/Device.h"

namespace Vulkan
{
    namespace Descriptor
    {
        DescriptorPool::DescriptorPool(const Device& deviceIn, const std::vector<DescriptorBinding>& descriptorBindings, size_t maxSets) :
            device(deviceIn)
        {
            std::vector<VkDescriptorPoolSize> poolSizes;

            for (const DescriptorBinding& binding : descriptorBindings) 
            {
                poolSizes.push_back(VkDescriptorPoolSize{ binding.type, static_cast<Platform::Type::Uint32>(binding.descriptorCount * maxSets) });
            }

            VkDescriptorPoolCreateInfo poolInfo = {};
            poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            poolInfo.poolSizeCount = static_cast<Platform::Type::Uint32>(poolSizes.size());
            poolInfo.pPoolSizes = poolSizes.data();
            poolInfo.maxSets = static_cast<Platform::Type::Uint32>(maxSets);

            CheckVKResult(vkCreateDescriptorPool(device.getHandle(), &poolInfo, nullptr, &descriptorPool), "create descriptor pool");
        }

        DescriptorPool::~DescriptorPool()
        {
            if (descriptorPool != nullptr) 
            {
                vkDestroyDescriptorPool(device.getHandle(), descriptorPool, nullptr);
                descriptorPool = nullptr;
            }
        }
    }
}