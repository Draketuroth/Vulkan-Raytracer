
#include "Vulkan/Descriptor/DescriptorSetManager.h"
#include "Vulkan/Descriptor/DescriptorPool.h"
#include "Vulkan/Descriptor/DescriptorSetLayout.h"
#include "Vulkan/Descriptor/DescriptorSets.h"
#include "Vulkan/Device.h"


namespace Vulkan
{
    namespace Descriptor
    {
        DescriptorSetManager::DescriptorSetManager(const Device& device, const std::vector<DescriptorBinding>& descriptorBindings, size_t maxSets)
        {
            // Perform sanity check to avoid accidentally binding different resources to the same binding point.
            std::map<Platform::Type::Uint32, VkDescriptorType> bindingTypes;

            for (const DescriptorBinding& binding : descriptorBindings) 
            {
                if (!bindingTypes.insert(std::make_pair(binding.binding, binding.type)).second) 
                {
                    throw std::invalid_argument("binding collision");
                }
            }

            descriptorPool.reset(new DescriptorPool(device, descriptorBindings, maxSets));
            descriptorSetLayout.reset(new class DescriptorSetLayout(device, descriptorBindings));
            descriptorSets.reset(new class DescriptorSets(*descriptorPool, *descriptorSetLayout, bindingTypes, maxSets));
        }

        DescriptorSetManager::~DescriptorSetManager()
        {
            descriptorSets.reset();
            descriptorSetLayout.reset();
            descriptorPool.reset();
        }
    }
}