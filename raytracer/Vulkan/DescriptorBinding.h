#ifndef VULKAN_DESCRIPTOR_BINDING_H
#define VULKAN_DESCRIPTOR_BINDING_H

#include "Vulkan/Core.h"
#include "Platform/StandardTypes.h"

namespace Vulkan 
{
    struct DescriptorBinding 
    {
        Platform::Type::Uint32 binding; // Slot to which the descriptor will be bound, which shall correspond to the layout index in the shader.
        Platform::Type::Uint32 descriptorCount; // Number of descriptors to bind.
        VkDescriptorType type; // Type of the bound descriptors.
        VkShaderStageFlags stage; // Shader stage at which the bound resources will first be available.
    };
}

#endif