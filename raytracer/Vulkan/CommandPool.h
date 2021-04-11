#ifndef VULKAN_COMMAND_POOL_H
#define VULKAN_COMMAND_POOL_H

namespace Vulkan { class Device; }

namespace Vulkan 
{
    class CommandPool final 
    {
    public:
        CommandPool(const Device& deviceIn, Platform::Type::Uint32 queueFamilyIndex, bool allowReset);
        ~CommandPool();

        const class Device& getDevice() const { return device; }

    private:

        const class Device& device;

        VkCommandPool commandPool;
    };
}

#endif