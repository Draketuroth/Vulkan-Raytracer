#ifndef VULKAN_INSTANCE_H
#define VULKAN_INSTANCE_H

namespace Core { class Window; }

namespace Vulkan 
{
    class Instance final 
    {
    public:
        Instance(const Core::Window& windowRef, const std::vector<const Platform::Type::Char*> validationLayersVec, Platform::Type::Uint32 vulkanVersion);
        ~Instance();

        const Core::Window& getWindow() const { return window; }

        const std::vector<VkExtensionProperties>& getExtensions() { return extensions; }
        const std::vector<VkLayerProperties>& getLayers() { return layers; }
        const std::vector<VkPhysicalDevice>& getDevices() { return devices; }
        const std::vector<const Platform::Type::Char*>& getValidationLayers() const { return validationLayers; }

        VkInstance getHandle() const { return instance; }

    private:

        void getVulkanExtensions();
        void getVulkanLayers();
        void getVulkanDevices();

        static void checkMinimumSupportedVulkanVersion(Platform::Type::Uint32 minimumVersion);
        static void checkVulkanValidationLayerSupport(const std::vector<const Platform::Type::Char*> validationLayersVec);

        const Core::Window& window;
        const std::vector<const Platform::Type::Char*> validationLayers;

        VkInstance instance;

        std::vector<VkExtensionProperties> extensions;
        std::vector<VkLayerProperties> layers;
        std::vector<VkPhysicalDevice> devices;
    };
}

#endif