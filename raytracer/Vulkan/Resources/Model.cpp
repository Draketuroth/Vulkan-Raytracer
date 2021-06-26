
#include "Vulkan/Resources/Model.h"

namespace Vulkan 
{
    namespace Resources 
    {
        void Model::setMaterial(const Material& material)
        {

        }

        void Model::transform(const glm::mat4 transform)
        {

        }

        Model::Model(std::vector<Vertex>&& verticesIn, std::vector<Platform::Type::Uint32>&& indicesIn, std::vector<Material>&& materialsIn, const class Procedural* proceduralIn) : 
            vertices(std::move(verticesIn)),
            indices(std::move(indicesIn)),
            materials(std::move(materialsIn)),
            procedural(proceduralIn)
        {

        }
    }
}