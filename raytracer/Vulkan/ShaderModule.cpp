
#include "Vulkan/ShaderModule.h"
#include "Vulkan/Device.h"
#include <fstream>

namespace Vulkan 
{
    ShaderModule::ShaderModule(const Device& deviceIn, const std::string& filename) :
        ShaderModule(deviceIn, readFile(filename))
    {

    }

    ShaderModule::ShaderModule(const Device& deviceIn, const std::vector<char>& code) :
        device(deviceIn)
    {
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const Platform::Type::Uint32*>(code.data());

        CheckVKResult(vkCreateShaderModule(device.getHandle(), &createInfo, nullptr, &shaderModule), "create shader module");
    }

    ShaderModule::~ShaderModule()
    {
        if (shaderModule != nullptr) 
        {
            vkDestroyShaderModule(device.getHandle(), shaderModule, nullptr);
            shaderModule = nullptr;
        }
    }

    VkPipelineShaderStageCreateInfo ShaderModule::createShaderStage(VkShaderStageFlagBits stage) const
    {
        VkPipelineShaderStageCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        createInfo.stage = stage;
        createInfo.shaderModule = shaderModule;
        createInfo.pName = "main";

        return createInfo;
    }

    std::vector<char> ShaderModule::readFile(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) 
        {
            throw std::runtime_error("failed to open file '" + filename + "'");
        }

        const auto fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;
    }
}