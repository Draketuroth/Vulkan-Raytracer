
#include "Vulkan/Core.h"
#include "Vulkan/CommandBuffers.h"
#include "Vulkan/CommandPool.h"
#include "Vulkan/Device.h"
#include <functional>

namespace Vulkan 
{
    class SingleTimeCommands final 
    {
    public:
        
        static void submit(CommandPool& commandPool, const std::function<void(VkCommandBuffer)>& action) 
        {
            CommandBuffers commandBuffers(commandPool, 1u);

            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

            vkBeginCommandBuffer(commandBuffers[0], &beginInfo);

            action(commandBuffers[0]);

            vkEndCommandBuffer(commandBuffers[0]);

            VkSubmitInfo submitInfo = {};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount = 1u;
            submitInfo.pCommandBuffers = &commandBuffers[0];

            const auto graphicsQueue = commandPool.getDevice().getGraphicsQueue();

            vkQueueSubmit(graphicsQueue, 1u, &submitInfo, nullptr);
            vkQueueWaitIdle(graphicsQueue);
        }
    };
}