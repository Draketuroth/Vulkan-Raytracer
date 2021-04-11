
#include "Vulkan/CommandPool.h"

#include "Vulkan/Device.h"

namespace Vulkan
{
    CommandPool::CommandPool(const Device& deviceIn, Platform::Type::Uint32 queueFamilyIndex, bool allowReset) :
        device(deviceIn) 
    {
        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndex;
        poolInfo.flags = allowReset ? VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT : 0;

        CheckVKResult(vkCreateCommandPool(device.getHandle(), &poolInfo, nullptr, &commandPool), "Create Command Pool");
    }

    CommandPool::~CommandPool()
    {
        if (commandPool != nullptr) 
        {
            vkDestroyCommandPool(device.getHandle(), commandPool, nullptr);
            commandPool = nullptr;
        }
    }
}