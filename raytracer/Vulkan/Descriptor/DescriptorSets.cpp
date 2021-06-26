
#include "Vulkan/Descriptor/DescriptorSets.h"
#include "Vulkan/Descriptor/DescriptorPool.h"
#include "Vulkan/Descriptor/DescriptorSetLayout.h"
#include "Vulkan/Device.h"

namespace Vulkan
{
    namespace Descriptor
    {
        DescriptorSets::DescriptorSets(
            const DescriptorPool& descriptorPoolIn, 
            const DescriptorSetLayout& layout, 
            std::map<Platform::Type::Uint32, VkDescriptorType> bindingTypesIn, 
            size_t size) :
            descriptorPool(descriptorPoolIn),
            bindingTypes(std::move(bindingTypesIn))
        {
            std::vector<VkDescriptorSetLayout> layouts(size, layout.getHandle());

            VkDescriptorSetAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = descriptorPool.getHandle();
            allocInfo.descriptorSetCount = static_cast<Platform::Type::Uint32>(size);
            allocInfo.pSetLayouts = layouts.data();

            descriptorSets.resize(size);

            CheckVKResult(vkAllocateDescriptorSets(descriptorPool.getDevice().getHandle(), &allocInfo, descriptorSets.data()), "allocate descriptor sets");
        }

        DescriptorSets::~DescriptorSets()
        {
            if (!descriptorSets.empty()) 
            {
                vkFreeDescriptorSets(
                    descriptorPool.getDevice().getHandle(),
                    descriptorPool.getHandle(),
                    static_cast<Platform::Type::Uint32>(descriptorSets.size()),
                    descriptorSets.data());

                descriptorSets.clear();
            }
        }

        VkWriteDescriptorSet DescriptorSets::bind(Platform::Type::Uint32 index, Platform::Type::Uint32 binding, const VkDescriptorBufferInfo& bufferInfo, Platform::Type::Uint32 count) const
        {
            VkWriteDescriptorSet descriptorWrite = {};
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = descriptorSets[index];
            descriptorWrite.dstBinding = binding;
            descriptorWrite.dstArrayElement = 0u;
            descriptorWrite.descriptorType = getBindingType(binding);
            descriptorWrite.descriptorCount = count;
            descriptorWrite.pBufferInfo = &bufferInfo;

            return descriptorWrite;
        }

        VkWriteDescriptorSet DescriptorSets::bind(Platform::Type::Uint32 index, Platform::Type::Uint32 binding, const VkDescriptorImageInfo& imageInfo, Platform::Type::Uint32 count) const
        {
            VkWriteDescriptorSet descriptorWrite = {};
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = descriptorSets[index];
            descriptorWrite.dstBinding = binding;
            descriptorWrite.dstArrayElement = 0u;
            descriptorWrite.descriptorType = getBindingType(binding);
            descriptorWrite.descriptorCount = count;
            descriptorWrite.pImageInfo = &imageInfo;

            return descriptorWrite;
        }

        VkWriteDescriptorSet DescriptorSets::bind(Platform::Type::Uint32 index, Platform::Type::Uint32 binding, const VkWriteDescriptorSetAccelerationStructureKHR& structureInfo, Platform::Type::Uint32 count) const
        {
            VkWriteDescriptorSet descriptorWrite = {};
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = descriptorSets[index];
            descriptorWrite.dstBinding = binding;
            descriptorWrite.dstArrayElement = 0u;
            descriptorWrite.descriptorType = getBindingType(binding);
            descriptorWrite.descriptorCount = count;
            descriptorWrite.pNext = &structureInfo;

            return descriptorWrite;
        }

        void DescriptorSets::updateDescriptors(Platform::Type::Uint32 index, const std::vector<VkWriteDescriptorSet>& descriptorWrites)
        {
            vkUpdateDescriptorSets(
                descriptorPool.getDevice().getHandle(),
                static_cast<Platform::Type::Uint32>(descriptorWrites.size()),
                descriptorWrites.data(), 0u, nullptr);
        }

        VkDescriptorType DescriptorSets::getBindingType(Platform::Type::Uint32 binding) const
        {
            const auto it = bindingTypes.find(binding);
            if (it == bindingTypes.end()) 
            {
                throw std::invalid_argument("binding not found!");
            }

            return it->second;
        }
    }
}