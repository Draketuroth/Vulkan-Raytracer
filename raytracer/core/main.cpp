
#include "Core/WindowProperties.h"
#include "Core/Raytracer.h"

#include "Vulkan/Debug/Info.h"
#include "Vulkan/Enumerate.h"

namespace 
{
	void SetVulkanDevice(Vulkan::Application& application);
}

struct Settings
{
	Platform::Type::Uint width = 1024u;
	Platform::Type::Uint height = 768u;
	bool fullScreen = false;
	VkPresentModeKHR presentMode = VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR;
};

void printHelpMessage() 
{
	std::cerr << "Usage: " << "Raytracer" << " <option(s)>\n"
		<< "Options:\n"
		<< "\t--help\t\tShow this help message\n"
		<< "\t--verbose\tPrint debug messages\n"
		<< std::endl;
}

int main(int argc, char** argv)
{

	try {
		Settings settings = Settings();

		// Parse command line arguments and configure application settings.
		for (Platform::Type::Int i = 1; i < argc; ++i) {
			std::string arg = argv[i];
			if ((arg == "-h") || (arg == "--help")) {
				printHelpMessage();
			}
		}

		// Prepare window configuration. @TODO: Set from configuration file.
		const Core::WindowProperties windowProperties("Vulkan Raytracer", settings.width, settings.height, settings.fullScreen);

		Core::Raytracer application(windowProperties, settings.presentMode);

		// Print information of the initialized Vulkan application.
		Vulkan::Debug::PrintVulkanSDKInformation();
		Vulkan::Debug::PrintVulkanInstanceInformation(application);
		Vulkan::Debug::PrintVulkanLayersInformation(application);
		Vulkan::Debug::PrintVulkanDevices(application);

		SetVulkanDevice(application);

		application.run();

		return EXIT_SUCCESS;
	}
	catch (const std::exception& exception) 
	{
		std::cerr << "FATAL: " << exception.what() << std::endl;
	}
	catch (...) 
	{
		std::cerr << "FATAL: Caught unhandled exception!" << std::endl;
		throw;
	}

	return EXIT_FAILURE;
}

namespace 
{
	void SetVulkanDevice(Vulkan::Application& application)
	{
		const auto& physicalDevices = application.devices();
		const auto result = std::find_if(physicalDevices.begin(), physicalDevices.end(), [](const VkPhysicalDevice& device)
			{
				VkPhysicalDeviceFeatures deviceFeatures;
				vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

				if (!deviceFeatures.geometryShader)
				{
					return false;
				}

				const auto queueFamilies = Vulkan::GetEnumerateVector(device, vkGetPhysicalDeviceQueueFamilyProperties);
				const auto hasGraphicsQueue = std::find_if(queueFamilies.begin(), queueFamilies.end(), [](const VkQueueFamilyProperties& queueFamily)
					{
						return queueFamily.queueCount > 0u && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT;
					});

				return hasGraphicsQueue != queueFamilies.end();
			});

		if (result == physicalDevices.end())
		{
			throw std::runtime_error("Cannot find a suitable device!");
		}

		VkPhysicalDeviceProperties2 deviceProperties{};
		deviceProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
		vkGetPhysicalDeviceProperties2(*result, &deviceProperties);

		std::cout << "Setting Device [" << deviceProperties.properties.deviceID << "]:" << std::endl;

		application.setPhysicalDevice(*result);

		std::cout << std::endl;
	}
}