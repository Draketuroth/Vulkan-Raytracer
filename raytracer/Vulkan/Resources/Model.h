#ifndef VULKAN_MODEL_H
#define VULKAN_MODEL_H

#include "Vulkan/Resources/Vertex.h"
#include "Vulkan/Resources/Material.h"

namespace Vulkan 
{
    namespace Resources
    {
        class Model final
        {
        public:

            void setMaterial(const Material& material);
            void transform(const glm::mat4 transform);

            const std::vector<Vertex>& getVertices() const { return vertices; }
            const std::vector<Platform::Type::Uint32>& getIndices() const { return indices; }
            const std::vector<Material>& getMaterials() const { return materials; }

            const class Procedural* getProcedural() const { return procedural.get(); }

            Platform::Type::Uint32 getNumVertices() const { return static_cast<Platform::Type::Uint32>(vertices.size()); }
            Platform::Type::Uint32 getNumIndices() const { return static_cast<Platform::Type::Uint32>(indices.size()); }
            Platform::Type::Uint32 getNumMaterials() const { return static_cast<Platform::Type::Uint32>(materials.size()); }

            Model() = default;
            Model(const Model&) = default;
            Model(Model&&) = default;
            ~Model() = default;

        private:
            Model& operator = (const Model&) = delete;
            Model& operator = (Model&&) = delete;

            Model(std::vector<Vertex>&& verticesIn, std::vector<Platform::Type::Uint32>&& indicesIn, std::vector<Material>&& materialsIn, const class Procedural* proceduralIn);

            std::vector<Vertex> vertices;
            std::vector<Platform::Type::Uint32> indices;
            std::vector<Material> materials;
            Platform::Pointer::Shared<const class Procedural> procedural;
        };
    }
}

#endif