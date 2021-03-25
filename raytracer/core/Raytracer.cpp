
#include "Core/Raytracer.h"

namespace 
{
    const bool EnableValidationLayer =
#ifdef NDEBUG
        false;
#else
        true;
#endif
}

namespace Core
{
    Raytracer::Raytracer(const Core::WindowProperties& windowProperties, VkPresentModeKHR presentMode) :
        Application(windowProperties, presentMode, EnableValidationLayer)
    {

    }

    Raytracer::~Raytracer()
    {

    }
}