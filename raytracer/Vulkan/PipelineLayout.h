#ifndef VULKAN_PIPELINE_LAYOUT_H
#define VULKAN_PIPELINE_LAYOUT_H

namespace Vulkan
{
    class DescriptorSetLayout;
    class Device;
}

namespace Vulkan
{
    class PipelineLayout final
    {
    public:
        PipelineLayout(const Device& deviceIn, const DescriptorSetLayout& descriptorSetLayout);
        ~PipelineLayout();

    private:
        PipelineLayout(const PipelineLayout&) = delete;
        PipelineLayout(PipelineLayout&&) = delete;
        PipelineLayout& operator = (const PipelineLayout&) = delete;
        PipelineLayout& operator = (PipelineLayout&&) = delete;

        const Device& device;

        VkPipelineLayout pipelineLayout;
    };
}

#endif