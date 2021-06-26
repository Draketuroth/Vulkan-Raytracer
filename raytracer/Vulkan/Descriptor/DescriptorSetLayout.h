#ifndef VULKAN_DESCRIPTOR_SET_LAYOUT_H
#define VULKAN_DESCRIPTOR_SET_LAYOUT_H

#include "Vulkan/Descriptor/DescriptorBinding.h"

namespace Vulkan { class Device; }

namespace Vulkan 
{
    namespace Descriptor 
    {
        class DescriptorSetLayout final
        {
        public:
            DescriptorSetLayout(const Device& deviceIn, const std::vector<DescriptorBinding>& descriptorBindings);
            ~DescriptorSetLayout();

            VkDescriptorSetLayout getHandle() const { return layout; }

        private:
            DescriptorSetLayout(const DescriptorSetLayout&) = delete;
            DescriptorSetLayout(DescriptorSetLayout&&) = delete;
            DescriptorSetLayout& operator = (const DescriptorSetLayout&) = delete;
            DescriptorSetLayout& operator = (DescriptorSetLayout&&) = delete;

            const Device& device;

            VkDescriptorSetLayout layout;
        };
    }
}

#endif