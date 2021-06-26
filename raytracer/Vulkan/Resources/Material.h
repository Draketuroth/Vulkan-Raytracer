#ifndef VULKAN_MATERIAL_H
#define VULKAN_MATERIAL_H

#include "Platform/StandardTypes.h"
#include "Core/Utilities/Glm.h"

namespace Vulkan 
{
    namespace Resources
    {
        struct alignas(16) Material final
        {
            // Vec3 and Vec4 gets aligned on 16 bytes in Vulkan shaders.

            static Material Lambertian(const glm::vec3& diffuse, const Platform::Type::Int32 textureId = -1)
            {
                return Material{ glm::vec4(diffuse, 1), textureId, 0.0f, 0.0f, MaterialModel::Lambertian };
            }

            static Material Metallic(const glm::vec3& diffuse, const float fuzziness, const Platform::Type::Int32 textureId = -1)
            {
                return Material{ glm::vec4(diffuse, 1), textureId, fuzziness, 0.0f, MaterialModel::Metallic };
            }

            static Material Dialectric(const float refractionIndex, const Platform::Type::Int32 textureId = -1)
            {
                return Material{ glm::vec4(0.7f, 0.7f, 1.0f, 1), textureId, 0.0f, refractionIndex, MaterialModel::Dielectric };
            }

            static Material Isotropic(const glm::vec3& diffuse, const Platform::Type::Int32 textureId = -1)
            {
                return Material{ glm::vec4(diffuse, 1), textureId, 0.0f, 0.0f, MaterialModel::Isotropic };
            }

            static Material DiffuseLight(const glm::vec3& diffuse, const Platform::Type::Int32 textureId = -1)
            {
                return Material{ glm::vec4(diffuse, 1), textureId, 0.0f, 0.0f, MaterialModel::DiffuseLight };
            }

            enum class MaterialModel : Platform::Type::Uint32
            {
                Lambertian = 0u,
                Metallic = 1u,
                Dielectric = 2u,
                Isotropic = 3u,
                DiffuseLight = 4u
            };

            // Base material
            glm::vec4 diffuse;
            Platform::Type::Int32 diffuseTextureId;

            // Metal fuzziness
            float fuzziness;

            // Dielectric refraction index
            float refractionIndex;

            // Which material is currently in use
            MaterialModel materialModel;
        };
    }
}

#endif