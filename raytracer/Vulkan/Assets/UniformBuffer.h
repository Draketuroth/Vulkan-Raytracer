#ifndef VULKAN_ASSETS_UNIFORM_BUFFER_H
#define VULKAN_ASSETS_UNIFORM_BUFFER_H

#include "Core/Utilities/Glm.h"

namespace 
{
    const float arr[16] = { 0.0f };
    const glm::mat4 zeroMat = glm::make_mat4(arr);
}

namespace Vulkan {
    class Buffer;
    class Device;
    class DeviceMemory;
}

namespace Vulkan 
{
    namespace Assets 
    {
        class UniformBufferObject 
        {
            UniformBufferObject() 
            {
                modelView = zeroMat;
                projection = zeroMat;
                modelViewInverse = zeroMat;
                projectionInverse = zeroMat;

                aperture = 0.0f;
                focusDistance = 0.0f;

                totalNumberOfSamples = 0u;
                numberOfSamples = 0u;
                numberOfBounces = 0u;
                randomSeed = 0u;
                hasSky = 0u;
            }

            glm::mat4 modelView;
            glm::mat4 projection;
            glm::mat4 modelViewInverse;
            glm::mat4 projectionInverse;

            Platform::Type::Float aperture;
            Platform::Type::Float focusDistance;

            Platform::Type::Uint32 totalNumberOfSamples;
            Platform::Type::Uint32 numberOfSamples;
            Platform::Type::Uint32 numberOfBounces;
            Platform::Type::Uint32 randomSeed;
            Platform::Type::Uint32 hasSky;
        };

        class UniformBuffer 
        {
        public:
            explicit UniformBuffer(const Vulkan::Device& device);
            UniformBuffer(UniformBuffer&& other) noexcept;
            ~UniformBuffer();

            const Vulkan::Buffer& getBuffer() const { return *buffer; }

            void setValue(const UniformBufferObject & ubo);

        private:

            Platform::Pointer::Scope<Vulkan::Buffer> buffer;
            Platform::Pointer::Scope<Vulkan::DeviceMemory> memory;
        };
    }
}

#endif