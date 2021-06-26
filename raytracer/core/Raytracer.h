#ifndef CORE_RAYTRACER_H
#define CORE_RAYTRACER_H

#include "Vulkan/Raytracing/Application.h"
#include "Vulkan/Resources/Scene.h"

namespace Core 
{
    class Raytracer final : public Vulkan::Raytracing::Application 
    {
    public:

        Raytracer(const Core::WindowProperties& windowProperties, VkPresentModeKHR presentMode);
        ~Raytracer();

    protected:

        const Vulkan::Resources::Scene& getScene() const override { return *scene; }

    private:

        Platform::Pointer::Scope<const Vulkan::Resources::Scene> scene;
    };
}

#endif
