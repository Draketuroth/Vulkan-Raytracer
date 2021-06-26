#ifndef VULKAN_DESCRIPTOR_SETS_H
#define VULKAN_DESCRIPTOR_SETS_H

namespace Vulkan 
{
    class Buffer;
    class ImageView;
}

namespace Vulkan 
{
    namespace Descriptor 
    {
        class DescriptorPool;
        class DescriptorSetLayout;
    }
}

namespace Vulkan
{
    namespace Descriptor
    {
        class DescriptorSets final 
        {
        public:
            DescriptorSets(
                const DescriptorPool& descriptorPoolIn,
                const DescriptorSetLayout& layout,
                std::map<Platform::Type::Uint32, VkDescriptorType> bindingTypesIn,
                size_t size
            );
            ~DescriptorSets();

            VkDescriptorSet getHandle(Platform::Type::Uint32 index) const { return descriptorSets[index]; }

            VkWriteDescriptorSet bind(Platform::Type::Uint32 index, Platform::Type::Uint32 binding, const VkDescriptorBufferInfo& bufferInfo, Platform::Type::Uint32 count = 1) const;
            VkWriteDescriptorSet bind(Platform::Type::Uint32 index, Platform::Type::Uint32 binding, const VkDescriptorImageInfo& imageInfo, Platform::Type::Uint32 count = 1) const;
            VkWriteDescriptorSet bind(Platform::Type::Uint32 index, Platform::Type::Uint32 binding, const VkWriteDescriptorSetAccelerationStructureKHR& structureInfo, Platform::Type::Uint32 count = 1) const;

            void updateDescriptors(Platform::Type::Uint32 index, const std::vector<VkWriteDescriptorSet>& descriptorWrites);

        private:
            DescriptorSets(const DescriptorSets&) = delete;
            DescriptorSets(DescriptorSets&&) = delete;
            DescriptorSets& operator = (const DescriptorSets&) = delete;
            DescriptorSets& operator = (DescriptorSets&&) = delete;

            VkDescriptorType getBindingType(Platform::Type::Uint32 binding) const;

            const DescriptorPool& descriptorPool;
            const std::map<Platform::Type::Uint32, VkDescriptorType> bindingTypes;

            std::vector<VkDescriptorSet> descriptorSets;
        };
    }
}

#endif