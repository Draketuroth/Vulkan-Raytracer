#ifndef VULKAN_SCENE_H
#define VULKAN_SCENE_H

namespace Vulkan 
{
    class Buffer;
    class CommandPool;
    class DeviceMemory;
    class Image;
}

namespace Vulkan 
{
    namespace Resources
    {
        class Model;
        class Texture;
        class TextureImage;
    }
}

namespace Vulkan 
{
    namespace Resources
    {
        class Scene final
        {
        public:

            Scene(Vulkan::CommandPool& commandPool, std::vector<Model>&& modelsIn, std::vector<Texture>&& texturesIn, bool usedForRayTracing);
            ~Scene();

            const std::vector<Model>& getModels() const { return models; }
            bool hasProcedurals() const { return static_cast<bool>(proceduralBuffer); }

            const Vulkan::Buffer& getVertexBuffer() const { return *vertexBuffer; }
            const Vulkan::Buffer& getIndexBuffer() const { return *indexBuffer; }
            const Vulkan::Buffer& getMaterialBuffer() const { return *materialBuffer; }
            const Vulkan::Buffer& getOffsetsBuffer() const { return *offsetBuffer; }
            const Vulkan::Buffer& getAabbBuffer() const { return *aabbBuffer; }
            const Vulkan::Buffer& getProceduralBuffer() const { return *proceduralBuffer; }
            const std::vector<VkImageView> getTextureImageViews() const { return textureImageViewHandles; }
            const std::vector<VkSampler> getTextureSamplers() const { return textureSamplerHandles; }

        private:
            Scene(const Scene&) = delete;
            Scene(Scene&&) = delete;
            Scene& operator = (const Scene&) = delete;
            Scene& operator = (Scene&&) = delete;

            const std::vector<Model> models;
            const std::vector<Texture> textures;

            Platform::Pointer::Scope<Vulkan::Buffer> vertexBuffer;
            Platform::Pointer::Scope<Vulkan::DeviceMemory> vertexBufferMemory;

            Platform::Pointer::Scope<Vulkan::Buffer> indexBuffer;
            Platform::Pointer::Scope<Vulkan::DeviceMemory> indexBufferMemory;

            Platform::Pointer::Scope<Vulkan::Buffer> materialBuffer;
            Platform::Pointer::Scope<Vulkan::DeviceMemory> materialBufferMemory;

            Platform::Pointer::Scope<Vulkan::Buffer> offsetBuffer;
            Platform::Pointer::Scope<Vulkan::DeviceMemory> offsetBufferMemory;
            
            Platform::Pointer::Scope<Vulkan::Buffer> aabbBuffer;
            Platform::Pointer::Scope<Vulkan::DeviceMemory> aabbBufferMemory;

            Platform::Pointer::Scope<Vulkan::Buffer> proceduralBuffer;
            Platform::Pointer::Scope<Vulkan::DeviceMemory> proceduralBufferMemory;

            std::vector<Platform::Pointer::Scope<TextureImage>> textureImages;
            std::vector<VkImageView> textureImageViewHandles;
            std::vector<VkSampler> textureSamplerHandles;
        };
    }
}

#endif