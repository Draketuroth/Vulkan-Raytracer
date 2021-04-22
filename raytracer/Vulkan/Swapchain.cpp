
#include "Vulkan/Swapchain.h"

#include "Vulkan/Device.h"
#include "Vulkan/Surface.h"
#include "Vulkan/Enumerate.h"
#include "Vulkan/Instance.h"
#include "Vulkan/ImageView.h"
#include "Vulkan/Debug/Utilities.h"

#include "Core/Window.h"

namespace Vulkan 
{
    Swapchain::Swapchain(const class Device& deviceIn, VkPresentModeKHR presentModeIn) :
        physicalDevice(deviceIn.getPhysicalDevice()),
        device(deviceIn)
    {
        const auto details = querySwapChainSupport(device.getPhysicalDevice(), device.getSurface().getHandle());
        if (details.formats.empty() || details.presentModes.empty())
        {
            throw std::runtime_error("Empty swap chain support!");
        }

        const Surface& surface = device.getSurface();
        const Core::Window& window = surface.getInstance().getWindow();

        const VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(details.formats);
        const VkPresentModeKHR actualPresentMode = chooseSwapPresentMode(details.presentModes, presentModeIn);
        const VkExtent2D swapExtent = chooseSwapExtent(window, details.capabilities);
        const Platform::Type::Uint32 imageCount = chooseImageCount(details.capabilities);

        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface.getHandle();
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1u;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        createInfo.preTransform = details.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = actualPresentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = nullptr;

        if (device.getGraphicsFamiliyIndex() != device.getPresentFamiliyIndex()) 
        {
            Platform::Type::Uint32 queueFamilyIndices[] = { device.getGraphicsFamiliyIndex(), device.getPresentFamiliyIndex() };

            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2u;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else 
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0u;
            createInfo.pQueueFamilyIndices = nullptr;
        }

        CheckVKResult(vkCreateSwapchainKHR(device.getHandle(), &createInfo, nullptr, &swapChain), "Create swap chain!");

        minImageCount = std::max(2u, details.capabilities.minImageCount);
        presentMode = actualPresentMode;
        format = surfaceFormat.format;
        extent = swapExtent;
        images = GetEnumerateVector(device.getHandle(), swapChain, vkGetSwapchainImagesKHR);

        for (const VkImage image : images) 
        {
            imageViews.push_back(std::make_unique<ImageView>(deviceIn, image, format, VK_IMAGE_ASPECT_COLOR_BIT));
        }

        const auto& debugUtils = device.getDebugUtilities();

        for (size_t i = 0; i != images.size(); ++i) 
        {
            debugUtils.setObjectName(images[i], ("Swapchain Image #" + std::to_string(i)).c_str());
            debugUtils.setObjectName(imageViews[i]->getHandle(), ("Swapchain ImageView #" + std::to_string(i)).c_str());
        }
    }

    Swapchain::~Swapchain()
    {
        imageViews.clear();

        if (swapChain != nullptr) 
        {
            vkDestroySwapchainKHR(device.getHandle(), swapChain, nullptr);
            swapChain = nullptr;
        }
    }

    Swapchain::SupportDetails Swapchain::querySwapChainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
    {
        SupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilities);
        details.formats = GetEnumerateVector(physicalDevice, surface, vkGetPhysicalDeviceSurfaceFormatsKHR);
        details.presentModes = GetEnumerateVector(physicalDevice, surface, vkGetPhysicalDeviceSurfacePresentModesKHR);

        return details;
    }
    VkSurfaceFormatKHR Swapchain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats)
    {
        if (formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED) 
        {
            return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
        }

        for (const auto& format : formats) 
        {
            if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
            {
                return format;
            }
        }

        throw std::runtime_error("Found no suitiable surface format!");
    }
    VkPresentModeKHR Swapchain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& presentModes, VkPresentModeKHR presentMode)
    {
        switch (presentMode)
        {
        case VK_PRESENT_MODE_IMMEDIATE_KHR:
        case VK_PRESENT_MODE_MAILBOX_KHR:
        case VK_PRESENT_MODE_FIFO_KHR:
        case VK_PRESENT_MODE_FIFO_RELAXED_KHR:

            if (std::find(presentModes.begin(), presentModes.end(), presentMode) != presentModes.end()) 
            {
                return presentMode;
            }

            break;

        default:
            throw std::out_of_range("Unknown present mode!");
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }
    VkExtent2D Swapchain::chooseSwapExtent(const Core::Window& window, const VkSurfaceCapabilitiesKHR& capabilites)
    {
        if (capabilites.currentExtent.width != std::numeric_limits<Platform::Type::Uint32>::max()) 
        {
            return capabilites.currentExtent;
        }

        VkExtent2D actualExtent = window.getFrameBufferSize();

        actualExtent.width = std::max(capabilites.minImageExtent.width, std::min(capabilites.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilites.minImageExtent.height, std::min(capabilites.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
    Platform::Type::Uint32 Swapchain::chooseImageCount(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        Platform::Type::Uint32 imageCount = std::max(2u, capabilities.minImageCount);

        if (capabilities.maxImageCount > 0u && imageCount > capabilities.maxImageCount) 
        {
            imageCount = capabilities.maxImageCount;
        }

        return Platform::Type::Uint32();
    }
}