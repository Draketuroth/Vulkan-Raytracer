#ifndef VULKAN_SWAPCHAIN_H
#define VULKAN_SWAPCHAIN_H

namespace Vulkan { class Device; class ImageView; }

namespace Core { class Window; }

namespace Vulkan 
{
    class Swapchain final 
    {
    public:

        Swapchain(const Device& deviceIn, VkPresentModeKHR presentModeIn);
        ~Swapchain();

        VkPhysicalDevice getPhysicalDevice() const { return physicalDevice; }
        const class Device& getDevice() const { return device; }

    private:

        struct SupportDetails 
        {
            VkSurfaceCapabilitiesKHR capabilities{};
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };

        static SupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
        static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
        static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& presentModes, VkPresentModeKHR presentMode);
        static VkExtent2D chooseSwapExtent(const Core::Window& window, const VkSurfaceCapabilitiesKHR& capabilites);
        static Platform::Type::Uint32 chooseImageCount(const VkSurfaceCapabilitiesKHR& capabilities);


        const VkPhysicalDevice physicalDevice;
        const class Device& device;

        VkSwapchainKHR swapChain;

        Platform::Type::Uint32 minImageCount;
        VkPresentModeKHR presentMode;
        VkFormat format;
        VkExtent2D extent {};
        std::vector<VkImage> images;
        std::vector<Platform::Pointer::Scope<ImageView>> imageViews;
    };
}

#endif