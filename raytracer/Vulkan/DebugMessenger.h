#ifndef VULKAN_DEBUG_MESSENGER_H
#define VULKAN_DEBUG_MESSENGER_H

namespace Vulkan 
{
    class Instance;

    class DebugMessenger final 
    {
    public:
        DebugMessenger(const Instance& instance, VkDebugUtilsMessageSeverityFlagBitsEXT threshold);
        ~DebugMessenger();

        VkDebugUtilsMessageSeverityFlagBitsEXT getThreshold() const { return threshold; }

    private:

        const Instance& instance;
        const VkDebugUtilsMessageSeverityFlagBitsEXT threshold;

        VkDebugUtilsMessengerEXT messenger;
    };
}

#endif