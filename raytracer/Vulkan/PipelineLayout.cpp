
#include "Vulkan/PipelineLayout.h"
#include "Vulkan/DescriptorSetLayout.h"

#include "Vulkan/Device.h"

namespace Vulkan 
{
    PipelineLayout::PipelineLayout(const Device& deviceIn, const DescriptorSetLayout& descriptorSetLayout) :
        device(deviceIn)
    {
        VkDescriptorSetLayout descriptorSetLayouts[] = { descriptorSetLayout.getHandle() };

        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1u;
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts;
        pipelineLayoutInfo.pushConstantRangeCount = 0u; // Optional.
        pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional.

        CheckVKResult(vkCreatePipelineLayout(device.getHandle(), &pipelineLayoutInfo, nullptr, &pipelineLayout), "Create pipeline layout");
    }

    PipelineLayout::~PipelineLayout()
    {
        if (pipelineLayout != nullptr) 
        {
            vkDestroyPipelineLayout(device.getHandle(), pipelineLayout, nullptr);
            pipelineLayout = nullptr;
        }
    }
}