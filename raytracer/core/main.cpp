
#include "Core/WindowProperties.h"
#include "Core/Raytracer.h"

#include "Vulkan/VulkanInfo.h"

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
		Vulkan::PrintVulkanSDKInformation();
		Vulkan::PrintVulkanInstanceInformation(application);
		Vulkan::PrintVulkanLayersInformation(application);
		Vulkan::PrintVulkanDevices(application);

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
	}

	return EXIT_FAILURE;
}