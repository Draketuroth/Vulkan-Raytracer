
#include "Vulkan/Instance.h"
#include "Vulkan/Version.h"
#include "Vulkan/Enumerate.h"

#include "Core/Window.h"

namespace Vulkan 
{
    Instance::Instance(const Core::Window& windowIn, const std::vector<const char*> validationLayersIn, Platform::Type::Uint32 vulkanVersion) :
        window(windowIn),
        validationLayers(validationLayersIn)
    {
        // Check the minimum supported Vulkan version on this machine.
        checkMinimumSupportedVulkanVersion(vulkanVersion);

        // Get a list of required extensions.
        std::vector<const char*> extensions = window.getRequiredInstanceExtensions();

        // Check the validation layers and add them to the required extensions.
        checkVulkanValidationLayerSupport(validationLayers);

        if (!validationLayers.empty()) 
        {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        // Create the Vulkan instance.
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Raytracer";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = vulkanVersion;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = static_cast<Platform::Type::Uint32>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();
        createInfo.enabledLayerCount = static_cast<Platform::Type::Uint32>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        CheckVKResult(vkCreateInstance(&createInfo, nullptr, &instance), "Create Vulkan instance");

        getVulkanDevices();
        getVulkanLayers();
        getVulkanExtensions();
    }

    Instance::~Instance()
    {
        if (instance != nullptr) 
        {
            vkDestroyInstance(instance, nullptr);
            instance = nullptr;
        }
    }

    void Instance::getVulkanExtensions()
    {
        GetEnumerateVector(static_cast<const char*>(nullptr), vkEnumerateInstanceExtensionProperties, extensions);
    }

    void Instance::getVulkanLayers()
    {
        GetEnumerateVector(vkEnumerateInstanceLayerProperties, layers);
    }

    void Instance::getVulkanDevices()
    {
        GetEnumerateVector(instance, vkEnumeratePhysicalDevices, devices);
        
        if (devices.empty()) 
        {
            throw std::runtime_error("Failed to find physical devices!");
        }
    }

    void Instance::checkMinimumSupportedVulkanVersion(Platform::Type::Uint32 minimumVersion)
    {
        Platform::Type::Uint32 version = 0;
        CheckVKResult(vkEnumerateInstanceVersion(&version), "Query Vulkan instance version");

        if (minimumVersion > version) 
        {
            std::ostringstream out;
            out << "Minimum required version not found (required " << Version(minimumVersion);
            out << "Found " << Version(minimumVersion);

            throw std::runtime_error(out.str());
        }
    }

    void Instance::checkVulkanValidationLayerSupport(const std::vector<const char*> validationLayersVec)
    {
        const auto availableLayers = GetEnumerateVector(vkEnumerateInstanceLayerProperties);

        /*
        Platform::Type::Uint32 layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
        */

        for (const char* layerName : validationLayersVec) 
        {
            auto result = std::find_if(availableLayers.begin(), availableLayers.end(), [layerName](const VkLayerProperties& layerProperties)
            {
                    return strcmp(layerName, layerProperties.layerName) == 0;
            });

            if (result == availableLayers.end()) 
            {
                throw std::runtime_error("Failed to find the requested validation layer: '" + std::string(layerName) + "'");
            }
        }
    }
}