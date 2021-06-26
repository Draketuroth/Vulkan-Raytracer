#ifndef VULKAN_SHADER_MODULE_H
#define VULKAN_SHADER_MODULE_H

namespace Vulkan { class Device; }

namespace Vulkan 
{
    class ShaderModule final 
    {
    public:

        ShaderModule(const Device& deviceIn, const std::string& filename);
        ShaderModule(const Device& deviceIn, const std::vector<char>& code);
        ~ShaderModule();

        const class Device& getDevice() const { return device; }

        VkPipelineShaderStageCreateInfo createShaderStage(VkShaderStageFlagBits stage) const;

    private:
        ShaderModule(const ShaderModule&) = delete;
        ShaderModule(ShaderModule&&) = delete;
        ShaderModule& operator = (const ShaderModule&) = delete;
        ShaderModule& operator = (ShaderModule&&) = delete;

        static std::vector<char> readFile(const std::string& filename);

        const class Device& device;

        VkShaderModule shaderModule;

    };
}

#endif