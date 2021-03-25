#ifndef CORE_RAYTRACER_H
#define CORE_RAYTRACER_H

#include "Vulkan/Raytracing/Application.h"

namespace Core 
{
    class Raytracer : public Vulkan::Raytracing::Application 
    {
    public:

        Raytracer(const Core::WindowProperties& windowProperties, VkPresentModeKHR presentMode);
        ~Raytracer();
    };
}

#endif
