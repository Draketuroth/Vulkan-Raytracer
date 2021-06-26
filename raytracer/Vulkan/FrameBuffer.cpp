
#include "Vulkan/FrameBuffer.h"
#include "Vulkan/DepthBuffer.h"
#include "Vulkan/Device.h"
#include "Vulkan/ImageView.h"
#include "Vulkan/RenderPass.h"
#include "Vulkan/SwapChain.h"

namespace Vulkan 
{
    FrameBuffer::FrameBuffer(const ImageView& imageViewIn, const RenderPass& renderPassIn) :
        imageView(imageViewIn),
        renderPass(renderPassIn) 
    {
        std::array<VkImageView, 2> attachments =
        {
            imageView.getHandle(),
            renderPass.getDepthBuffer().getImageView().getHandle()
        };

        VkFramebufferCreateInfo frameBufferInfo = {};
        frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frameBufferInfo.renderPass = renderPass.getHandle();
        frameBufferInfo.attachmentCount = static_cast<Platform::Type::Uint32>(attachments.size());
        frameBufferInfo.pAttachments = attachments.data();
        frameBufferInfo.width = renderPass.getSwapChain().getExtent().width;
        frameBufferInfo.height = renderPass.getSwapChain().getExtent().height;
        frameBufferInfo.layers = 1u;

        CheckVKResult(vkCreateFramebuffer(imageView.getDevice().getHandle(), &frameBufferInfo, nullptr, &frameBuffer), "create frame buffer");
    }

    FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept :
        imageView(other.imageView),
        renderPass(other.renderPass),
        frameBuffer(other.frameBuffer)
    {
        other.frameBuffer = nullptr;
    }

    FrameBuffer::~FrameBuffer()
    {
        if (frameBuffer != nullptr) 
        {
            vkDestroyFramebuffer(imageView.getDevice().getHandle(), frameBuffer, nullptr);
            frameBuffer = nullptr;
        }
    }
}