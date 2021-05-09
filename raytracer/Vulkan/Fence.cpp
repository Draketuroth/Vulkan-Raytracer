
#include "Vulkan/Fence.h"
#include "Vulkan/Device.h"

namespace Vulkan 
{
    Fence::Fence(const Device& deviceIn, bool signaled) :
        device(deviceIn)
    {
        VkFenceCreateInfo fenceInfo = {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

        CheckVKResult(vkCreateFence(device.getHandle(), &fenceInfo, nullptr, &fence), "Create fence");

    }

    Fence::Fence(Fence&& other) noexcept :
        device(other.device),
        fence(other.fence)
    {
        other.fence = nullptr;
    }

    Fence::~Fence()
    {
        if (fence != nullptr) 
        {
            vkDestroyFence(device.getHandle(), fence, nullptr);
            fence = nullptr;
        }
    }

    void Fence::reset()
    {
        CheckVKResult(vkResetFences(device.getHandle(), 1u, &fence), "Reset fence");
    }

    void Fence::wait(Platform::Type::Uint64 timeout) const
    {
        CheckVKResult(vkWaitForFences(device.getHandle(), 1u, &fence, VK_TRUE, timeout), "Wait for fence");
    }
}