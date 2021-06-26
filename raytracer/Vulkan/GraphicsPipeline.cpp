
#include "Vulkan/GraphicsPipeline.h"
#include "Vulkan/Buffer.h"
#include "Vulkan/Descriptor/DescriptorSetManager.h"
#include "Vulkan/Descriptor/DescriptorPool.h"
#include "Vulkan/Descriptor/DescriptorSets.h"
#include "Vulkan/Device.h"
#include "Vulkan/PipelineLayout.h"
#include "Vulkan/RenderPass.h"
#include "Vulkan/ShaderModule.h"
#include "Vulkan/SwapChain.h"

#include "Vulkan/Resources/Scene.h"
#include "Vulkan/Resources/UniformBuffer.h"
#include "Vulkan/Resources/Vertex.h"

namespace Vulkan 
{
    GraphicsPipeline::GraphicsPipeline(const SwapChain& swapChainIn, const DepthBuffer& depthBuffer, const std::vector<Resources::UniformBuffer>& uniformBuffers, const Resources::Scene& scene, bool isWireFrame) :
        swapChain(swapChainIn),
        wireframe(isWireFrame)
    {
        const Device& device = swapChainIn.getDevice();
        const VkVertexInputBindingDescription bindingDescription = Resources::Vertex::getBindingDescription();
        const auto attributeDescription = Resources::Vertex::getAttributeDescription();

        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 1u;
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<Platform::Type::Uint32>(attributeDescription.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescription.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapChain.getExtent().width);
        viewport.height = static_cast<float>(swapChain.getExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor = {};
        scissor.offset = { 0, 0 };
        scissor.extent = swapChain.getExtent();

        VkPipelineViewportStateCreateInfo viewportState = {};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1u;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1u;
        viewportState.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer = {};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = isWireFrame ? VK_POLYGON_MODE_LINE : VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f; // Optional
        rasterizer.depthBiasClamp = 0.0f; // Optional
        rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

        VkPipelineMultisampleStateCreateInfo multiSampling = {};
        multiSampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multiSampling.sampleShadingEnable = VK_FALSE;
        multiSampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multiSampling.minSampleShading = 1.0f; // Optional
        multiSampling.pSampleMask = nullptr; // Optional;
        multiSampling.alphaToCoverageEnable = VK_FALSE; // Optional
        multiSampling.alphaToOneEnable = VK_FALSE;

        VkPipelineDepthStencilStateCreateInfo depthStencil = {};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.minDepthBounds = 0.0f; // Optional
        depthStencil.maxDepthBounds = 1.0f; // optional
        depthStencil.stencilTestEnable = VK_FALSE; 
        depthStencil.front = {}; // Optional
        depthStencil.back = {}; // Optional

        VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

        VkPipelineColorBlendStateCreateInfo colorBlending = {};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
        colorBlending.attachmentCount = 1u;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f; // Optional
        colorBlending.blendConstants[1] = 0.0f; // Optional
        colorBlending.blendConstants[2] = 0.0f; // Optional
        colorBlending.blendConstants[3] = 0.0f; // Optional

        // Create descriptor pool/sets
        // @TODO : Initalize scene...
        std::vector<Descriptor::DescriptorBinding> descriptorBindings =
        {
            {0u, 1u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
            {1u, 1u, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT},
            {2u, static_cast<Platform::Type::Uint32>(scene.getTextureSamplers().size()), VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
        };

        descriptorSetManager.reset(new Descriptor::DescriptorSetManager(device, descriptorBindings, uniformBuffers.size()));

        Descriptor::DescriptorSets& descriptorSets = descriptorSetManager->getDescriptorSets();

        const Platform::Type::Uint32 numImages = static_cast<Platform::Type::Uint32>(swapChain.getImages().size());
        for (Platform::Type::Uint32 i = 0u; i != numImages; i++)
        {
            // Uniform buffer
            VkDescriptorBufferInfo uniformBufferInfo = {};
            uniformBufferInfo.buffer = uniformBuffers[i].getBuffer().getHandle();
            uniformBufferInfo.range = VK_WHOLE_SIZE;

            // Material buffer
            VkDescriptorBufferInfo materialBufferInfo = {};
            materialBufferInfo.buffer = scene.getMaterialBuffer().getHandle();
            materialBufferInfo.range = VK_WHOLE_SIZE;

            // Image and texture samplers
            std::vector<VkDescriptorImageInfo> imageInfos(scene.getTextureSamplers().size());

            for (size_t t = 0; t != imageInfos.size(); ++t) 
            {
                VkDescriptorImageInfo& imageInfo = imageInfos[t];
                imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo.imageView = scene.getTextureImageViews()[t];
                imageInfo.sampler = scene.getTextureSamplers()[t];
            }

            const std::vector<VkWriteDescriptorSet> descriptorWrites =
            {
                descriptorSets.bind(i, 0u, uniformBufferInfo),
                descriptorSets.bind(i, 1u, materialBufferInfo),
                descriptorSets.bind(i, 2u, *imageInfos.data(), static_cast<Platform::Type::Uint32>(imageInfos.size()))
            };

            descriptorSets.updateDescriptors(i, descriptorWrites);
        }

        // Create pipeline layout and render pass.
        pipelineLayout.reset(new class PipelineLayout(device, descriptorSetManager->getDescriptorLayout()));
        renderPass.reset(new class RenderPass(swapChain, depthBuffer, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_LOAD_OP_CLEAR));

        // Load shaders.
        const ShaderModule vertexShader(device, "Assets\\Shaders\\Compiled\\Graphics.vert.spv");
        const ShaderModule fragmentShader(device, "Assets\\Shaders\\Compiled\\Graphics.frag.spv");

        VkPipelineShaderStageCreateInfo shaderStages[] =
        {
            vertexShader.createShaderStage(VK_SHADER_STAGE_VERTEX_BIT),
            fragmentShader.createShaderStage(VK_SHADER_STAGE_FRAGMENT_BIT)
        };

        // Create graphic pipeline.
        VkGraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2u;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multiSampling;
        pipelineInfo.pDepthStencilState = &depthStencil;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = nullptr; // Optional
        pipelineInfo.basePipelineHandle = nullptr; // Optional
        pipelineInfo.basePipelineIndex - 1; // Optional
        pipelineInfo.layout = pipelineLayout->getHandle();
        pipelineInfo.renderPass = renderPass->getHandle();
        pipelineInfo.subpass = 0u;

        CheckVKResult(vkCreateGraphicsPipelines(device.getHandle(), nullptr, 1u, &pipelineInfo, nullptr, &pipeline), "create graphics pipeline");
    }

    GraphicsPipeline::~GraphicsPipeline()
    {
        if (pipeline != nullptr) 
        {
            vkDestroyPipeline(swapChain.getDevice().getHandle(), pipeline, nullptr);
            pipeline = nullptr;
        }

        renderPass.reset();
        pipelineLayout.reset();
        descriptorSetManager.reset();
    }

    VkDescriptorSet GraphicsPipeline::getDescriptorSet(Platform::Type::Uint32 index) const
    {
        return descriptorSetManager->getDescriptorSets().getHandle(index);
    }
}