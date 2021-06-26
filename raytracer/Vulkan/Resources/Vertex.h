#ifndef VULKAN_VERTEX_H
#define VULKAN_VERTEX_H

#include "Platform/StandardTypes.h"
#include "Core/Utilities/Glm.h"
#include "Vulkan/Core.h"

#include <array>

namespace Vulkan 
{
    namespace Resources 
    {
        struct Vertex final 
        {
            bool operator==(const Vertex& other) const 
            {
                return
                    pos == other.pos &&
                    norm == other.norm &&
                    texCoord == other.texCoord &&
                    matIndex == other.matIndex;
            }

            static VkVertexInputBindingDescription getBindingDescription() 
            {
                VkVertexInputBindingDescription bindingDescription = {};
                bindingDescription.binding = 0u;
                bindingDescription.stride = sizeof(Vertex);
                bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
                return bindingDescription;
            }

            static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescription()
            {
                std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions = {};
                
                // Position
                attributeDescriptions[0].binding = 0u;
                attributeDescriptions[0].location = 0u;
                attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
                attributeDescriptions[0].offset = offsetof(Vertex, pos);

                // Normal
                attributeDescriptions[1].binding = 0u;
                attributeDescriptions[1].location = 1u;
                attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
                attributeDescriptions[1].offset = offsetof(Vertex, norm);

                // Texture coordinates
                attributeDescriptions[2].binding = 0u;
                attributeDescriptions[2].location = 2u;
                attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
                attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

                // Material index
                attributeDescriptions[3].binding = 0u;
                attributeDescriptions[3].location = 3u;
                attributeDescriptions[3].format = VK_FORMAT_R32_SINT;
                attributeDescriptions[3].offset = offsetof(Vertex, matIndex);

                return attributeDescriptions;
            }

            glm::vec3 pos;
            glm::vec3 norm;
            glm::vec2 texCoord;
            Platform::Type::Uint32 matIndex;
        };
    }
}

#endif