#ifndef VULKAN_SURFACE_H
#define VULKAN_SURFACE_H

namespace Core { class Window; }

namespace Vulkan { class Instance; }

namespace Vulkan
{
    class Surface final 
    {
    public:
        explicit Surface(const Instance& instanceIn);
        ~Surface();

        const class Instance& getInstance() const { return instance; }
        VkSurfaceKHR getHandle() const { return surface; }

    private:

        const class Instance& instance;

        VkSurfaceKHR surface;
    };
}

#endif