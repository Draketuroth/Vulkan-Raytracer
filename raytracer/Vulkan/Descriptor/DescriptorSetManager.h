#ifndef VULKAN_DESCRIPTOR_SET_MANAGER_H
#define VULKAN_DESCRIPTOR_SET_MANAGER_H

#include "Vulkan/Descriptor/DescriptorBinding.h"

namespace Vulkan { class Device; }

namespace Vulkan 
{
    namespace Descriptor 
    {
        class DescriptorPool;
        class DescriptorSetLayout;
        class DescriptorSets;
    }
}

namespace Vulkan 
{
    namespace Descriptor 
    {
        class DescriptorSetManager final 
        {
        public:
            explicit DescriptorSetManager(const Device& device, const std::vector<DescriptorBinding>& descriptorBindings, size_t maxSets);
            ~DescriptorSetManager();

            const class DescriptorSetLayout& getDescriptorLayout() const { return *descriptorSetLayout; }
            class DescriptorSets& getDescriptorSets() { return *descriptorSets; }


        private:
            DescriptorSetManager(const DescriptorSetManager&) = delete;
            DescriptorSetManager(DescriptorSetManager&&) = delete;
            DescriptorSetManager& operator = (const DescriptorSetManager&) = delete;
            DescriptorSetManager& operator = (DescriptorSetManager&&) = delete;

            Platform::Pointer::Scope<class DescriptorPool> descriptorPool;
            Platform::Pointer::Scope<class DescriptorSetLayout> descriptorSetLayout;
            Platform::Pointer::Scope<class DescriptorSets> descriptorSets;
        };
    }
}

#endif