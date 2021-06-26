#ifndef VULKAN_RENDERPASS_H
#define VULKAN_RENDERPASS_H

namespace Vulkan 
{
    class DepthBuffer;
    class SwapChain;
}

namespace Vulkan 
{
    class RenderPass final 
    {
    public:
        RenderPass(const SwapChain& swapChain,
            const DepthBuffer& depthBuffer,
            VkAttachmentLoadOp colorBufferLoadOp,
            VkAttachmentLoadOp depthBufferLoadOp);

        ~RenderPass();

        VkRenderPass getHandle() const { return renderPass; }

        const class SwapChain& getSwapChain() const { return swapChain; }
        const class DepthBuffer& getDepthBuffer() const { return depthBuffer; }

    private:
        RenderPass(const RenderPass&) = delete;
        RenderPass(RenderPass&&) = delete;
        RenderPass& operator = (const RenderPass&) = delete;
        RenderPass& operator = (RenderPass&&) = delete;

        const class SwapChain& swapChain;
        const class DepthBuffer& depthBuffer;

        VkRenderPass renderPass;
    };
}

#endif