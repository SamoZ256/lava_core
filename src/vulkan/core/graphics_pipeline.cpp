#include "vulkan/lvcore/core/graphics_pipeline.hpp"

#include "vulkan/lvcore/core/device.hpp"
#include "vulkan/lvcore/core/swap_chain.hpp"
#include "vulkan/lvcore/core/descriptor_set.hpp"

namespace lv {

Vulkan_GraphicsPipeline::Vulkan_GraphicsPipeline(Vulkan_GraphicsPipelineCreateInfo& createInfo) : pipelineLayout(*createInfo.pipelineLayout)/*, pushConstantRanges(createInfo.pushConstantRanges.data()), pushConstantRangeCount(createInfo.pushConstantRanges.size())*/ {
    //Available shaders
    //geometryShader = (geometryFilename != nullptr);
    //createPipelineLayout(descriptorSetLayouts);

    //createPipelineLayout();

    //bool renderPassSet = (createInfo.renderPass != nullptr);
    if (createInfo.renderPass == nullptr)
        throw std::runtime_error("You must specify a valid render pass");
    Vulkan_PipelineConfigInfo configInfo = defaultPipelineConfigInfo(createInfo.config, createInfo);
    //unsigned int depthTest = createInfo.config.depthTest;//(framebufferSet ? (createInfo.framebuffer->depthAttachment == nullptr ? VK_FALSE : VK_TRUE) : VK_TRUE);
    /*
    if (renderPassSet) {
        if (!createInfo.renderPass->hasDepthAttachment)
            depthTest = VK_FALSE;
    } else depthTest = VK_FALSE;
    */
    configInfo.depthStencilInfo.depthTestEnable = createInfo.config.depthTestEnable;
    configInfo.depthStencilInfo.depthWriteEnable = (createInfo.config.depthTestEnable == VK_TRUE ? createInfo.config.depthWriteEnable : VK_FALSE);
    //std::cout << "Depth write: " << configInfo.depthStencilInfo.depthWriteEnable << std::endl;

    configInfo.renderPass = createInfo.renderPass->renderPass;
    configInfo.pipelineLayout = pipelineLayout.pipelineLayout;

    //std::string vertexCode = readFile(createInfo.vertexFilename);
    //std::string fragmentCode = readFile(createInfo.fragmentFilename);
    //std::string geometryCode = geometryShader ? readFile(geometryFilename) : "";

    //createShaderModule(vertexCode, &vertexShaderModule, VK_SHADER_STAGE_VERTEX_BIT, createInfo.vertexConstants, createInfo.vertexConstantsSize, createInfo.vertexConstantsData);
    //createShaderModule(fragmentCode, &fragmentShaderModule, VK_SHADER_STAGE_FRAGMENT_BIT, createInfo.fragmentConstants, createInfo.fragmentConstantsSize, createInfo.fragmentConstantsData);
    //if (geometryShader) createShaderModule(geometryCode, &geometryShaderModule, VK_SHADER_STAGE_GEOMETRY_BIT);

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    if (createInfo.vertexDescriptor != nullptr) {
        vertexInputInfo.vertexBindingDescriptionCount = createInfo.vertexDescriptor->vertexDesc.bindingDescriptions.size();
        vertexInputInfo.pVertexBindingDescriptions = createInfo.vertexDescriptor->vertexDesc.bindingDescriptions.data();
        vertexInputInfo.vertexAttributeDescriptionCount = createInfo.vertexDescriptor->vertexDesc.attributeDescriptions.size();
        vertexInputInfo.pVertexAttributeDescriptions = createInfo.vertexDescriptor->vertexDesc.attributeDescriptions.data();
    }

    /*
    std::vector<VkVertexInputBindingDescription> bindingDescriptions;
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;

    if (vertexType == VERTEX_TYPE_MAIN) {
        bindingDescriptions = Vertex::getBindingDescriptions();
        attributeDescriptions = Vertex::getAttributeDescriptions();

        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
        vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();
    } else if (vertexType == VERTEX_TYPE_NONE) {
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr;
        vertexInputInfo.pVertexBindingDescriptions = nullptr;
    } else {
        throw std::invalid_argument("Unsupported vertex type");
    }
    */
    //std::cout << "VECTOR SIZE: " << attributeDescriptions.size() << std::endl;

    std::vector<VkPipelineShaderStageCreateInfo> shaderStages(2);
    shaderStages[0] = createInfo.vertexShaderModule->stageInfo;
    shaderStages[1] = createInfo.fragmentShaderModule->stageInfo;
    //if (geometryShader) shaderStages.push_back(geometryShaderModule.stageInfo);

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = shaderStages.size();
    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
    pipelineInfo.pViewportState = &configInfo.viewportInfo;
    pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo;
    pipelineInfo.pMultisampleState = &configInfo.multisampleInfo;
    pipelineInfo.pColorBlendState = &configInfo.colorBlendInfo;
    pipelineInfo.pDepthStencilState = &configInfo.depthStencilInfo;
    pipelineInfo.pDynamicState = &configInfo.dynamicStateInfo;

    pipelineInfo.layout = configInfo.pipelineLayout;
    pipelineInfo.renderPass = configInfo.renderPass;
    pipelineInfo.subpass = configInfo.subpass;

    pipelineInfo.basePipelineIndex = -1;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    VK_CHECK_RESULT(vkCreateGraphicsPipelines(g_vulkan_device->device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline))
}

void Vulkan_GraphicsPipeline::destroy() {
    //vkDestroyPipelineLayout(g_metal_device->device(), pipelineLayout, nullptr);
    //vkDestroyShaderModule(g_metal_device->device(), vertexShaderModule.ID, nullptr);
    //vkDestroyShaderModule(g_metal_device->device(), fragmentShaderModule.ID, nullptr);
    //if (geometryShader) vkDestroyShaderModule(g_metal_device->device(), geometryShaderModule.ID, nullptr);
    vkDestroyPipeline(g_vulkan_device->device(), graphicsPipeline, nullptr);
}

void Vulkan_GraphicsPipeline::uploadPushConstants(void* data, uint8_t index) {
    vkCmdPushConstants(g_vulkan_swapChain->getActiveCommandBuffer(),
                      pipelineLayout.pipelineLayout,
                      pipelineLayout.pushConstantRanges[index].stageFlags,
                      pipelineLayout.pushConstantRanges[index].offset,
                      pipelineLayout.pushConstantRanges[index].size, data);
}

void Vulkan_GraphicsPipeline::bind() {
    vkCmdBindPipeline(g_vulkan_swapChain->getActiveCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
    g_vulkan_swapChain->pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    //std::cout << "DEPENDENCY COUNT: " << (int)g_metal_swapChain->activeFramebuffer->renderPass->dependencies.size() << std::endl;
}

Vulkan_PipelineConfigInfo Vulkan_GraphicsPipeline::defaultPipelineConfigInfo(Vulkan_GraphicsPipelineConfig& config, Vulkan_GraphicsPipelineCreateInfo& graphicsPipelineCreateInfo) {
    Vulkan_PipelineConfigInfo configInfo;

    //Input assembly
    configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

    //Viewport
    configInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    configInfo.viewportInfo.viewportCount = 1;
    configInfo.viewportInfo.pViewports = nullptr;
    configInfo.viewportInfo.scissorCount = 1;
    configInfo.viewportInfo.pScissors = nullptr;

    //Rasterization
    configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
    configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
    configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
    configInfo.rasterizationInfo.lineWidth = 1.0f;
    configInfo.rasterizationInfo.cullMode = config.cullMode;
    configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
    configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
    configInfo.rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
    configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

    //Multisample
    configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
    configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    configInfo.multisampleInfo.minSampleShading = 1.0f;           // Optional
    configInfo.multisampleInfo.pSampleMask = nullptr;             // Optional
    configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
    configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

    //Color blending (attachment)
    std::vector<Vulkan_RenderPassAttachment>& colorAttachments = graphicsPipelineCreateInfo.renderPass->colorAttachments;
    configInfo.colorBlendAttachments.resize(colorAttachments.size());
    for (uint8_t i = 0; i < colorAttachments.size(); i++) {
        uint8_t index = colorAttachments[i].index;
        //std::cout << "Blending enabled: " << (enabled.blend ? "true" : "false") << std::endl;
        configInfo.colorBlendAttachments[index].colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT;
        configInfo.colorBlendAttachments[index].blendEnable = colorAttachments[i].blendEnable;
        configInfo.colorBlendAttachments[index].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        configInfo.colorBlendAttachments[index].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        configInfo.colorBlendAttachments[index].colorBlendOp = VK_BLEND_OP_ADD;
        configInfo.colorBlendAttachments[index].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        configInfo.colorBlendAttachments[index].dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        configInfo.colorBlendAttachments[index].alphaBlendOp = VK_BLEND_OP_MAX;
    }

    //Color blending (info)
    configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
    configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
    configInfo.colorBlendInfo.attachmentCount = configInfo.colorBlendAttachments.size();
    configInfo.colorBlendInfo.pAttachments = configInfo.colorBlendAttachments.data();
    configInfo.colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
    configInfo.colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
    configInfo.colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
    configInfo.colorBlendInfo.blendConstants[3] = 0.0f;  // Optional

    //Depth and stencil
    configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    //configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
    //configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
    configInfo.depthStencilInfo.depthCompareOp = config.depthOp;
    configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
    configInfo.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
    configInfo.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
    configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
    configInfo.depthStencilInfo.front = {};  // Optional
    configInfo.depthStencilInfo.back = {};   // Optional

    //Dynamic state
    configInfo.dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
    configInfo.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    configInfo.dynamicStateInfo.pDynamicStates = configInfo.dynamicStates.data();
    configInfo.dynamicStateInfo.dynamicStateCount = (uint32_t)configInfo.dynamicStates.size();
    configInfo.dynamicStateInfo.flags = 0;

    return configInfo;
}

} //namespace lv
