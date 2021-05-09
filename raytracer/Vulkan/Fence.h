#ifndef VULKAN_FENCE_H
#define VULKAN_FENCE_H

namespace Vulkan { class Device; }

namespace Vulkan 
{
    class Fence final 
    {
    public:

        explicit Fence(const Device& deviceIn, bool signaled);
        Fence(Fence&& other) noexcept;
        ~Fence();

        const Device& getDevice() const { return device; }
        const VkFence getHandle() const { return fence; }

        void reset();
        void wait(Platform::Type::Uint64 timeout) const;

    private:

        const Device& device;

        VkFence fence{};
    };
}

#endif