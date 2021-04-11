
#include "Vulkan/Debug/Utilities.h"

namespace Vulkan
{
    namespace Debug
    {
        Utilities::Utilities(VkInstance instance) :
            vkSetDebugUtilsObjectNameEXT(reinterpret_cast<PFN_vkSetDebugUtilsObjectNameEXT>(vkGetInstanceProcAddr(instance, "vkSetDebugUtilsObjectNameEXT")))
        {
#ifndef NDEBUG
            if (vkSetDebugUtilsObjectNameEXT == nullptr) 
            {
                throw std::runtime_error("Failed to get address of 'vkSetDebugUtilsObjectNameEXT'");
            }
#endif
        }
    }
}