
#include "Vulkan/Semaphore.h"
#include "Device.h"

namespace Vulkan 
{
    Semaphore::Semaphore(const Device& deviceIn) : 
        device(deviceIn)
    {
        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        CheckVKResult(vkCreateSemaphore(device.getHandle(), &semaphoreInfo, nullptr, &semaphore), "Create semaphore");
    }

    Semaphore::Semaphore(Semaphore&& other) noexcept :
        device(other.device),
        semaphore(other.semaphore)
    {
        other.semaphore = nullptr;
    }

    Semaphore::~Semaphore()
    {
        if (semaphore != nullptr) 
        {
            vkDestroySemaphore(device.getHandle(), semaphore, nullptr);
            semaphore = nullptr;
        }
    }
}