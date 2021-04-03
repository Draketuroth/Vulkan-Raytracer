
#include "Vulkan/DebugMessenger.h"

#include "Vulkan/Instance.h"

namespace Vulkan 
{
    const Platform::Type::Char* ObjectTypeToString(const VkObjectType objectType)
    {
        switch (objectType)
        {
#define STR(object) case VK_OBJECT_TYPE_ ## object: return # object
            STR(UNKNOWN);
            STR(INSTANCE);
            STR(PHYSICAL_DEVICE);
            STR(DEVICE);
            STR(QUEUE);
            STR(SEMAPHORE);
            STR(COMMAND_BUFFER);
            STR(FENCE);
            STR(DEVICE_MEMORY);
            STR(BUFFER);
            STR(IMAGE);
            STR(EVENT);
            STR(QUERY_POOL);
            STR(BUFFER_VIEW);
            STR(IMAGE_VIEW);
            STR(SHADER_MODULE);
            STR(PIPELINE_CACHE);
            STR(PIPELINE_LAYOUT);
            STR(RENDER_PASS);
            STR(PIPELINE);
            STR(DESCRIPTOR_SET_LAYOUT);
            STR(SAMPLER);
            STR(DESCRIPTOR_POOL);
            STR(DESCRIPTOR_SET);
            STR(FRAMEBUFFER);
            STR(COMMAND_POOL);
            STR(SAMPLER_YCBCR_CONVERSION);
            STR(DESCRIPTOR_UPDATE_TEMPLATE);
            STR(SURFACE_KHR);
            STR(SWAPCHAIN_KHR);
            STR(DISPLAY_KHR);
            STR(DISPLAY_MODE_KHR);
            STR(DEBUG_REPORT_CALLBACK_EXT);
            STR(DEBUG_UTILS_MESSENGER_EXT);
            STR(ACCELERATION_STRUCTURE_KHR);
            STR(VALIDATION_CACHE_EXT);
            STR(PERFORMANCE_CONFIGURATION_INTEL);
            STR(DEFERRED_OPERATION_KHR);
            STR(INDIRECT_COMMANDS_LAYOUT_NV);
        default:
            return "Unknown Object Type!";
        }
    }

    namespace 
    {
        VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
            const VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            const VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* const callbackData,
            void* const userData) 
        {
            (void)userData;

            switch (messageSeverity)
            {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
                std::cerr << "VERBOSE: ";
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
                std::cerr << "INFO: ";
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
                std::cerr << "WARNING: ";
                break;
            default:
                std::cerr << "UNKNOWN: ";
            }

            switch (messageType)
            {
            case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
                std::cerr << "GENERAL: ";
                break;
            case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
                std::cerr << "VALIDATION: ";
                break;
            case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
                std::cerr << "PERFORMANCE: ";
                break;
            default:
                std::cerr << "GENERAL: ";
            }

            std::cerr << callbackData->pMessage;

            if (callbackData->objectCount > 0 && messageSeverity > VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) 
            {
                for (Platform::Type::Uint32 i = 0u; i != callbackData->objectCount; ++i) 
                {
                    const VkDebugUtilsObjectNameInfoEXT object = callbackData->pObjects[i];
                    std::cerr
                        << "  - Object[" << i << "]: "
                        << "Type: " << ObjectTypeToString(object.objectType) << ", "
                        << "Handle: " << reinterpret_cast<void*>(object.objectHandle) << ", "
                        << "Name: '" << (object.pObjectName ? object.pObjectName : "") << "'"
                        << "\n";
                }
            }

            std::cerr << std::endl;

            return VK_FALSE;
        }

        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* createInfo, const VkAllocationCallbacks* allocator, VkDebugUtilsMessengerEXT* callback) 
        {
            const auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
            return func != nullptr 
                ? func(instance, createInfo, allocator, callback)
                : VK_ERROR_EXTENSION_NOT_PRESENT;
        }

        void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* allocator) 
        {
            const auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
            if (func != nullptr) 
            {
                func(instance, callback, allocator);
            }
        }
    }

    DebugMessenger::DebugMessenger(const Instance& instanceIn, VkDebugUtilsMessageSeverityFlagBitsEXT thresholdIn) :
        instance(instanceIn),
        threshold(thresholdIn)
    {
        if (instance.getValidationLayers().empty()) 
        {
            return;
        }

        VkDebugUtilsMessageSeverityFlagsEXT severity = 0;

        switch (threshold) 
        {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            severity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            severity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            severity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            severity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            break;
        default:
            throw std::invalid_argument("Invalid debug utils threshold!");
        }

        VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = severity;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = VulkanDebugCallback;
        createInfo.pUserData = nullptr;

        CheckVKResult(CreateDebugUtilsMessengerEXT(instance.getHandle(), &createInfo, nullptr, &messenger), "Setting up Vulkan debug callback");
    }
    DebugMessenger::~DebugMessenger()
    {
        if (messenger != nullptr) 
        {
            DestroyDebugUtilsMessengerEXT(instance.getHandle(), messenger, nullptr);
            messenger = nullptr;
        }
    }
}