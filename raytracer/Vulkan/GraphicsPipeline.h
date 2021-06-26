#ifndef VULKAN_GRAPHICS_PIPELINE
#define VULKAN_GRAPHICS_PIPELINE

namespace Vulkan 
{
    class DepthBuffer;
    class PipelineLayout;
    class RenderPass;
    class SwapChain;

    namespace Resources 
    {
        class Scene;
        class UniformBuffer;
    }

    namespace Descriptor 
    {
        class DescriptorSetManager;
    }
}

namespace Vulkan 
{
    class GraphicsPipeline final 
    {
    public:

        GraphicsPipeline(
            const SwapChain& swapChainIn,
            const DepthBuffer& depthBuffer,
            const std::vector<Resources::UniformBuffer>& uniformBuffers,
            const Resources::Scene& scene,
            bool isWireFrame);
        ~GraphicsPipeline();

        VkDescriptorSet getDescriptorSet(Platform::Type::Uint32 index) const;
        bool isWireframe() const { return wireframe; }
        const class PipelineLayout& getPipelineLayout() const { return *pipelineLayout; }
        const class RenderPass& getRenderPass() const { return *renderPass; }

    private:
        GraphicsPipeline(const GraphicsPipeline&) = delete;
        GraphicsPipeline(GraphicsPipeline&&) = delete;
        GraphicsPipeline& operator = (const GraphicsPipeline&) = delete;
        GraphicsPipeline& operator = (GraphicsPipeline&&) = delete;

        const SwapChain& swapChain;
        const bool wireframe;

        VkPipeline pipeline;

        Platform::Pointer::Scope<Descriptor::DescriptorSetManager> descriptorSetManager;
        Platform::Pointer::Scope<class PipelineLayout> pipelineLayout;
        Platform::Pointer::Scope<class RenderPass> renderPass;
    };
}

#endif