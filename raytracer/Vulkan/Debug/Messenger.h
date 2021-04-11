#ifndef VULKAN_DEBUG_MESSENGER_H
#define VULKAN_DEBUG_MESSENGER_H

namespace Vulkan 
{
    class Instance;

    namespace Debug 
    {
        class Messenger final
        {
        public:
            Messenger(const Instance& instance, VkDebugUtilsMessageSeverityFlagBitsEXT threshold);
            ~Messenger();

            VkDebugUtilsMessageSeverityFlagBitsEXT getThreshold() const { return threshold; }

        private:

            const Instance& instance;
            const VkDebugUtilsMessageSeverityFlagBitsEXT threshold;

            VkDebugUtilsMessengerEXT messenger;
        };
    }
}

#endif