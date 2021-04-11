#ifndef VULKAN_DEBUG_UTILITIES
#define VULKAN_DEBUG_UTILIITIES

namespace Vulkan 
{
    namespace Debug 
    {
        class Utilities final 
        {
        public:
            explicit Utilities(VkInstance instance);
            ~Utilities() = default;

            void setDevice(VkDevice deviceIn) { device = deviceIn; }

            void setObjectName(const VkAccelerationStructureKHR& object, const Platform::Type::Char* name) const { setObjectName(object, name, VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR); }
            void setObjectName(const VkBuffer& object, const Platform::Type::Char* name) const { setObjectName(object, name, VK_OBJECT_TYPE_BUFFER); }
            void setObjectName(const VkCommandBuffer& object, const Platform::Type::Char* name) const { setObjectName(object, name, VK_OBJECT_TYPE_COMMAND_BUFFER); }
            void setObjectName(const VkDescriptorSet& object, const Platform::Type::Char* name) const { setObjectName(object, name, VK_OBJECT_TYPE_DESCRIPTOR_SET); };
            void setObjectName(const VkDescriptorSetLayout& object, const Platform::Type::Char* name) const { setObjectName(object, name, VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT); };
            void setObjectName(const VkDeviceMemory& object, const Platform::Type::Char* name) const { setObjectName(object, name, VK_OBJECT_TYPE_DEVICE_MEMORY); };
            void setObjectName(const VkFramebuffer& object, const Platform::Type::Char* name) const { setObjectName(object, name, VK_OBJECT_TYPE_FRAMEBUFFER); };
            void setObjectName(const VkImage& object, const Platform::Type::Char* name) const { setObjectName(object, name, VK_OBJECT_TYPE_IMAGE); };
            void setObjectName(const VkImageView& object, const Platform::Type::Char* name) const { setObjectName(object, name, VK_OBJECT_TYPE_IMAGE_VIEW); };
            void setObjectName(const VkPipeline& object, const Platform::Type::Char* name) const { setObjectName(object, name, VK_OBJECT_TYPE_PIPELINE); };
            void setObjectName(const VkQueue& object, const Platform::Type::Char* name) const { setObjectName(object, name, VK_OBJECT_TYPE_QUEUE); };
            void setObjectName(const VkRenderPass& object, const Platform::Type::Char* name) const { setObjectName(object, name, VK_OBJECT_TYPE_RENDER_PASS); };
            void setObjectName(const VkSemaphore& object, const Platform::Type::Char* name) const { setObjectName(object, name, VK_OBJECT_TYPE_SEMAPHORE); };
            void setObjectName(const VkShaderModule& object, const Platform::Type::Char* name) const { setObjectName(object, name, VK_OBJECT_TYPE_SHADER_MODULE); };
            void setObjectName(const VkSwapchainKHR& object, const Platform::Type::Char* name) const { setObjectName(object, name, VK_OBJECT_TYPE_SWAPCHAIN_KHR); };

        private:
            template <typename T>
            void setObjectName(const T& object, const Platform::Type::Char* name, VkObjectType type) const 
            {
#ifndef NDEBUG
                VkDebugUtilsObjectNameInfoEXT info = {};
                info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
                info.pNext = nullptr;
                info.objectHandle = reinterpret_cast<const Platform::Type::Uint64&>(object);
                info.objectType = type;
                info.pObjectName = name;

                CheckVKResult(vkSetDebugUtilsObjectNameEXT(device, &info), "Set object name");
#endif
            }

            const PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT;

            VkDevice device{};
        };
    }
}

#endif