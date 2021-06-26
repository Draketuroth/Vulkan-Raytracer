
#include "Vulkan/Resources/Scene.h"
#include "Vulkan/Resources/Model.h"
#include "Vulkan/Resources/Sphere.h"
#include "Vulkan/Resources/Texture.h"
#include "Vulkan/Resources/TextureImage.h"
#include "Vulkan/BufferUtilities.h"
#include "Vulkan/ImageView.h"
#include "Vulkan/Sampler.h"
#include "Vulkan/SingleTimeCommands.h"

namespace Vulkan
{
    namespace Resources 
    {
        Scene::Scene(Vulkan::CommandPool& commandPool, std::vector<Model>&& modelsIn, std::vector<Texture>&& texturesIn, bool usedForRayTracing) :
            models(std::move(modelsIn)),
            textures(std::move(texturesIn))
        {
            // Concatenate all the models in the scene.
            std::vector<Vertex> vertices;
            std::vector<Platform::Type::Uint32> indices;
            std::vector<Material> materials;
            std::vector<glm::vec4> procedurals;
            std::vector<VkAabbPositionsKHR> aabbs;
            std::vector<glm::uvec2> offsets;

            for (const Model& model : models) 
            {
                // Keep track of the index, vertex and material offsets.
                const Platform::Type::Uint indexOffset = static_cast<Platform::Type::Uint>(indices.size());
                const Platform::Type::Uint vertexOffset = static_cast<Platform::Type::Uint>(vertices.size());
                const Platform::Type::Int32 materialOffset = static_cast<Platform::Type::Int32>(materials.size());

                offsets.emplace_back(indexOffset, vertexOffset);

                // Iterate each model and copy their data.
               const std::vector<Vertex>& vertexIt = model.getVertices();
               const std::vector<Platform::Type::Uint32>& indexIt = model.getIndices();
               const std::vector<Material>& materialIt = model.getMaterials();

               vertices.insert(vertices.end(), vertexIt.begin(), vertexIt.end());
               indices.insert(indices.end(), indexIt.begin(), indexIt.end());
               materials.insert(materials.end(), materialIt.begin(), materialIt.end());

               const Platform::Type::Uint numVertices = static_cast<Platform::Type::Uint>(vertices.size());

               // Adjust the material index.
               for (Platform::Type::Uint i = vertexOffset; i != numVertices; ++i) 
               {
                   vertices[i].matIndex += materialOffset;
               }

               // Add optional procedurals.
               const Sphere* const sphere = dynamic_cast<const Sphere*>(model.getProcedural());
               if (sphere != nullptr) 
               {
                   const std::pair<glm::vec3, glm::vec3> aabb = sphere->boundingBox();
                   aabbs.push_back({ aabb.first.x, aabb.first.y, aabb.first.z, aabb.second.x, aabb.second.y, aabb.second.z });
                   procedurals.emplace_back(sphere->center, sphere->radius);
               }
               else
               {
                   aabbs.emplace_back();
                   procedurals.emplace_back();
               }
            }

            const auto flag = usedForRayTracing ? VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT : 0;

            Vulkan::BufferUtilities::CreateDeviceBuffer(commandPool, "Vertices", VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | flag, vertices, vertexBuffer, vertexBufferMemory);
            Vulkan::BufferUtilities::CreateDeviceBuffer(commandPool, "Indices", VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | flag, indices, indexBuffer, indexBufferMemory);
            Vulkan::BufferUtilities::CreateDeviceBuffer(commandPool, "Materials", flag, materials, materialBuffer, materialBufferMemory);
            Vulkan::BufferUtilities::CreateDeviceBuffer(commandPool, "Offsets", flag, offsets, offsetBuffer, offsetBufferMemory);

            Vulkan::BufferUtilities::CreateDeviceBuffer(commandPool, "AABBs", flag, aabbs, aabbBuffer, aabbBufferMemory);
            Vulkan::BufferUtilities::CreateDeviceBuffer(commandPool, "Procedurals", flag, procedurals, proceduralBuffer, proceduralBufferMemory);

            // Upload all textures.
            textureImages.reserve(textures.size());
            textureImageViewHandles.resize(textures.size());
            textureSamplerHandles.resize(textures.size());

            const Platform::Type::Uint numTextures = static_cast<Platform::Type::Uint>(textures.size());

            for (Platform::Type::Uint i = 0u; i != numTextures; ++i) 
            {
                textureImages.emplace_back(new TextureImage(commandPool, textures[i]));
                textureImageViewHandles[i] = textureImages[i]->getImageView().getHandle();
                textureSamplerHandles[i] = textureImages[i]->getSampler().getHandle();
            }
        }

        Scene::~Scene()
        {
            textureSamplerHandles.clear();
            textureImageViewHandles.clear();
            textureImages.clear();

            proceduralBuffer.reset();
            proceduralBufferMemory.reset();

            aabbBuffer.reset();
            aabbBufferMemory.reset();

            offsetBuffer.reset();
            offsetBufferMemory.reset();

            materialBuffer.reset();
            materialBufferMemory.reset();

            indexBuffer.reset();
            indexBufferMemory.reset();

            vertexBuffer.reset();
            vertexBufferMemory.reset();
        }
    }
}


