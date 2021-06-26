#ifndef VULKAN_FRAME_BUFFER_H
#define VULKAN_FRAME_BUFFER_H

namespace Vulkan 
{
    class ImageView;
    class RenderPass;
}

namespace Vulkan 
{
    class FrameBuffer final 
    {
    public:
        explicit FrameBuffer(const ImageView& imageViewIn, const RenderPass& renderPassIn);
        FrameBuffer(FrameBuffer&& other) noexcept;
        ~FrameBuffer();

        const class ImageView& getImageView() const { return imageView; }
        const class RenderPass& getRenderPass() const { return renderPass; }

    private:
        FrameBuffer(const FrameBuffer&) = delete;
        FrameBuffer& operator = (const FrameBuffer&) = delete;
        FrameBuffer& operator = (FrameBuffer&&) = delete;

        const class ImageView& imageView;
        const class RenderPass& renderPass;

        VkFramebuffer frameBuffer;
    };
}

#endif