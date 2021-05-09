#ifndef VULKAN_SEMAPHORE_H
#define VULKAN_SEMAPHORE_H

namespace Vulkan { class Device; }

namespace Vulkan 
{
    class Semaphore final 
    {
    public:

        explicit Semaphore(const Device& deviceIn);
        Semaphore(Semaphore&& other) noexcept;
        ~Semaphore();

        const class Device& getDevice() const { return device; }
        VkSemaphore getHandle() const { return semaphore; }

    private:

        const class Device& device;

        VkSemaphore semaphore;
    };
}

#endif