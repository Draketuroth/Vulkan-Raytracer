#ifndef VULKAN_SPHERE_H
#define VULKAN_SPHERE_H

#include "Vulkan/Resources/Procedural.h"
#include "Core/Utilities/Glm.h"

namespace Vulkan 
{
    namespace Resources 
    {
        class Sphere final : public Procedural 
        {
        public:
            Sphere(const glm::vec3& centerIn, const float radiusIn) :
                center(centerIn), radius(radiusIn) 
            {
            }

            const glm::vec3 center;
            const float radius;

            std::pair<glm::vec3, glm::vec3> boundingBox() const override 
            {
                return std::make_pair(center - radius, center + radius);
            }
        };
    }
}

#endif