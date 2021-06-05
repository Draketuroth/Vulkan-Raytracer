
#include "Vulkan/DescriptorSetLayout.h"

#include "Vulkan/Device.h"

namespace Vulkan
{
    DescriptorSetLayout::DescriptorSetLayout(const Device& deviceIn, const std::vector<DescriptorBinding>& descriptorBindings) :
        device(deviceIn)
    {
        std::vector<VkDescriptorSetLayoutBinding> layoutBindings;

        for (const auto& descriptorBinding : descriptorBindings) 
        {
            VkDescriptorSetLayoutBinding layoutBinding = {};
            layoutBinding.binding = descriptorBinding.binding;
            layoutBinding.descriptorCount = descriptorBinding.descriptorCount;
            layoutBinding.descriptorType = descriptorBinding.type;
            layoutBinding.stageFlags = descriptorBinding.stage;

            layoutBindings.push_back(layoutBinding);
        }

        VkDescriptorSetLayoutCreateInfo layoutInfo = {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<Platform::Type::Uint32>(layoutBindings.size());
        layoutInfo.pBindings = layoutBindings.data();

        CheckVKResult(vkCreateDescriptorSetLayout(device.getHandle(), &layoutInfo, nullptr, &layout), "Create descriptor set layout");
    }

    DescriptorSetLayout::~DescriptorSetLayout()
    {
        if (layout != nullptr) 
        {
            vkDestroyDescriptorSetLayout(device.getHandle(), layout, nullptr);
            layout = nullptr;
        }
    }
}