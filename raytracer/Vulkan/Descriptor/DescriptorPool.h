#ifndef VULKAN_DESCRIPTOR_POOL
#define VULKAN_DESCRIPTOR_POOL

#include "Vulkan/Descriptor/DescriptorBinding.h"

namespace Vulkan { class Device; }

namespace Vulkan
{
    namespace Descriptor
    {
        class DescriptorPool final 
        {
        public:
            DescriptorPool(const Device& deviceIn, const std::vector<DescriptorBinding>& descriptorBindings, size_t maxSets);
            ~DescriptorPool();

            const class Device& getDevice() const { return device; }
            VkDescriptorPool getHandle() const { return descriptorPool; }

        private:
            DescriptorPool(const DescriptorPool&) = delete;
            DescriptorPool(DescriptorPool&&) = delete;
            DescriptorPool& operator = (const DescriptorPool&) = delete;
            DescriptorPool& operator = (DescriptorPool&&) = delete;

            const class Device& device;

            VkDescriptorPool descriptorPool;
        };
    }
}

#endif