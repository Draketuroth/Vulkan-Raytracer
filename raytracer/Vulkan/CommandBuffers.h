#ifndef VULKAN_COMMAND_BUFFERS_H
#define VULKAN_COMMAND_BUFFERS_H

namespace Vulkan { class CommandPool; }

namespace Vulkan 
{
    class CommandBuffers final 
    {
    public:

        CommandBuffers(CommandPool& commandPoolIn, Platform::Type::Uint32 size);
        ~CommandBuffers();

        Platform::Type::Uint32 size() const { return static_cast<Platform::Type::Uint32>(commandBuffers.size()); }
        VkCommandBuffer& operator [] (const size_t i) { return commandBuffers[i]; }

        VkCommandBuffer begin(size_t i);
        void end(size_t i);

    private:

        const CommandPool& commandPool;

        std::vector<VkCommandBuffer> commandBuffers;
    };

}

#endif