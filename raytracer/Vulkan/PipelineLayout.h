#ifndef VULKAN_PIPELINE_LAYOUT_H
#define VULKAN_PIPELINE_LAYOUT_H

namespace Vulkan
{
    class Device;
}

namespace Vulkan 
{
    namespace Descriptor 
    {
        class DescriptorSetLayout;
    }
}

namespace Vulkan
{
    class PipelineLayout final
    {
    public:
        PipelineLayout(const Device& deviceIn, const Descriptor::DescriptorSetLayout& descriptorSetLayout);
        ~PipelineLayout();

        VkPipelineLayout getHandle() const { return pipelineLayout; }

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