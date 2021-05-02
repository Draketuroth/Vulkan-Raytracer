
#include "Vulkan/CommandBuffers.h"

#include "Vulkan/CommandPool.h"
#include "Vulkan/Device.h"

namespace Vulkan 
{
    CommandBuffers::CommandBuffers(CommandPool& commandPoolIn, Platform::Type::Uint32 size) :
        commandPool(commandPoolIn)
    {
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool.getHandle();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = size;

        commandBuffers.resize(size);

        CheckVKResult(vkAllocateCommandBuffers(commandPool.getDevice().getHandle(), &allocInfo, commandBuffers.data()), "Allocate command buffers");
    }

    CommandBuffers::~CommandBuffers()
    {
        if (!commandBuffers.empty()) 
        {
            vkFreeCommandBuffers(commandPool.getDevice().getHandle(), commandPool.getHandle(), static_cast<Platform::Type::Uint32>(commandBuffers.size()), commandBuffers.data());
            commandBuffers.clear();
        }
    }

    VkCommandBuffer CommandBuffers::begin(size_t i)
    {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        beginInfo.pInheritanceInfo = nullptr;

        CheckVKResult(vkBeginCommandBuffer(commandBuffers[i], &beginInfo), "Begin recording command buffer");

        return commandBuffers[i];
    }

    void CommandBuffers::end(size_t i)
    {
        CheckVKResult(vkEndCommandBuffer(commandBuffers[i]), "Record command buffer");
    }
}