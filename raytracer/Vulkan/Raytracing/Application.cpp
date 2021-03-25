
#include "Vulkan/Raytracing/Application.h"

namespace Vulkan
{
    namespace Raytracing
    {
        Application::Application(const Core::WindowProperties& windowProperties, VkPresentModeKHR presentMode, bool enableValidationLayers) :
            Vulkan::Application(windowProperties, presentMode, enableValidationLayers)
        {

        }

        Application::~Application() 
        {

        }
    }
}
