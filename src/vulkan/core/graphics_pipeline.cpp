#include "vulkan/lvcore/core/graphics_pipeline.hpp"

#include "vulkan/lvcore/core/device.hpp"
#include "vulkan/lvcore/core/swap_chain.hpp"
#include "vulkan/lvcore/core/descriptor_set.hpp"

namespace lv {

void Vulkan_GraphicsPipeline::init() {
    if (renderPass == nullptr)
        throw std::runtime_error("You must specify a valid render pass");
    
    compile();
}

void Vulkan_GraphicsPipeline::destroy() {
    vkDestroyPipeline(g_vulkan_device->device(), graphicsPipeline, nullptr);
}

void Vulkan_GraphicsPipeline::compile() {
    Vulkan_PipelineConfigInfo configInfo = defaultPipelineConfigInfo();

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    if (vertexDescriptor != nullptr) {
        vertexInputInfo.vertexBindingDescriptionCount = vertexDescriptor->vertexDesc.bindingDescriptions.size();
        vertexInputInfo.pVertexBindingDescriptions = vertexDescriptor->vertexDesc.bindingDescriptions.data();
        vertexInputInfo.vertexAttributeDescriptionCount = vertexDescriptor->vertexDesc.attributeDescriptions.size();
        vertexInputInfo.pVertexAttributeDescriptions = vertexDescriptor->vertexDesc.attributeDescriptions.data();
    }

    std::vector<VkPipelineShaderStageCreateInfo> shaderStages(2);
    shaderStages[0] = vertexShaderModule->stageInfo;
    shaderStages[1] = fragmentShaderModule->stageInfo;

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

    pipelineInfo.layout = pipelineLayout->pipelineLayout;
    pipelineInfo.renderPass = renderPass->renderPass;
    pipelineInfo.subpass = subpassIndex;

    pipelineInfo.basePipelineIndex = -1;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    VK_CHECK_RESULT(vkCreateGraphicsPipelines(g_vulkan_device->device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline));
}

void Vulkan_GraphicsPipeline::recompile() {
    destroy();
    compile();
}

void Vulkan_GraphicsPipeline::uploadPushConstants(void* data, uint8_t index) {
    vkCmdPushConstants(g_vulkan_swapChain->getActiveCommandBuffer(),
                      pipelineLayout->pipelineLayout,
                      pipelineLayout->pushConstantRanges[index].stageFlags,
                      pipelineLayout->pushConstantRanges[index].offset,
                      pipelineLayout->pushConstantRanges[index].size, data);
}

void Vulkan_GraphicsPipeline::bind() {
    vkCmdBindPipeline(g_vulkan_swapChain->getActiveCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
    g_vulkan_swapChain->pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    //std::cout << "DEPENDENCY COUNT: " << (int)g_metal_swapChain->activeFramebuffer->renderPass->dependencies.size() << std::endl;
}

Vulkan_PipelineConfigInfo Vulkan_GraphicsPipeline::defaultPipelineConfigInfo() {
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
    configInfo.colorBlendAttachments.resize(colorBlendAttachments.size());
    for (uint8_t i = 0; i < colorBlendAttachments.size(); i++) {
        //uint8_t index = i;//colorBlendAttachments[i].index;
        configInfo.colorBlendAttachments[i].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        configInfo.colorBlendAttachments[i].blendEnable = colorBlendAttachments[i].blendEnable;
        configInfo.colorBlendAttachments[i].srcColorBlendFactor = colorBlendAttachments[i].srcRgbBlendFactor;
        configInfo.colorBlendAttachments[i].dstColorBlendFactor = colorBlendAttachments[i].dstRgbBlendFactor;
        configInfo.colorBlendAttachments[i].colorBlendOp = colorBlendAttachments[i].rgbBlendOp;
        configInfo.colorBlendAttachments[i].srcAlphaBlendFactor = colorBlendAttachments[i].srcAlphaBlendFactor;
        configInfo.colorBlendAttachments[i].dstAlphaBlendFactor = colorBlendAttachments[i].dstAlphaBlendFactor;
        configInfo.colorBlendAttachments[i].alphaBlendOp = colorBlendAttachments[i].alphaBlendOp;
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
    configInfo.depthStencilInfo.depthTestEnable = config.depthTestEnable;
    configInfo.depthStencilInfo.depthWriteEnable = (config.depthTestEnable == VK_TRUE ? config.depthWriteEnable : VK_FALSE);
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
