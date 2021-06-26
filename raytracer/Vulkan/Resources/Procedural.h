#ifndef VULKAN_PROCEDURAL_H
#define VULKAN_PROCEDURAL_H

#include "Core/Utilities/Glm.h"
#include <utility>

namespace Vulkan 
{
    namespace Resources 
    {
        class Procedural 
        {
        public:
            Procedural() = default;
            virtual ~Procedural() = default;
            virtual std::pair<glm::vec3, glm::vec3> boundingBox() const = 0;

        private:
            Procedural(const Procedural&) = delete;
            Procedural(Procedural&&) = delete;
            Procedural& operator = (const Procedural&) = delete;
            Procedural& operator = (Procedural&) = delete;
        };
    }
}

#endif