#ifndef LV_VULKAN_GRAPHICS_PIPELINE_H
#define LV_VULKAN_GRAPHICS_PIPELINE_H

#include <cassert>

#include "render_pass.hpp"
#include "descriptor_set.hpp"
#include "vertex_descriptor.hpp"
#include "shader_module.hpp"

namespace lv {

struct Vulkan_PipelineConfigInfo {
    VkPipelineViewportStateCreateInfo viewportInfo{};
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
    VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
    VkPipelineMultisampleStateCreateInfo multisampleInfo{};
    std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments{};
    VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
    std::vector<VkDynamicState> dynamicStates;
    VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
};

struct Vulkan_GraphicsPipelineConfig {
    VkCullModeFlagBits cullMode = VK_CULL_MODE_NONE;
    unsigned int depthTestEnable = VK_FALSE;
    unsigned int depthWriteEnable = VK_TRUE;
    VkCompareOp depthOp = VK_COMPARE_OP_LESS;
};

class Vulkan_GraphicsPipeline {
public:
    VkPipeline graphicsPipeline;

    Vulkan_ShaderModule* vertexShaderModule;
    Vulkan_ShaderModule* fragmentShaderModule;
    Vulkan_PipelineLayout* pipelineLayout;
    Vulkan_RenderPass* renderPass;
    uint8_t subpassIndex = 0;
    Vulkan_VertexDescriptor* vertexDescriptor = nullptr;
    Vulkan_GraphicsPipelineConfig config;
    std::vector<Vulkan_ColorBlendAttachment> colorBlendAttachments;

    void init();

    void destroy();

    void compile();

    void recompile();

    void uploadPushConstants(void* data, uint8_t index);

    void bind();

    void addColorBlendAttachment(Vulkan_ColorBlendAttachment attachment) { colorBlendAttachments.push_back(attachment); }

    Vulkan_PipelineConfigInfo defaultPipelineConfigInfo();
};

} //namespace lv

#endif
