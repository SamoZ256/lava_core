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

struct Vulkan_GraphicsPipelineCreateInfo {
    Vulkan_ShaderModule* vertexShaderModule;
    Vulkan_ShaderModule* fragmentShaderModule;
    Vulkan_PipelineLayout* pipelineLayout;
    Vulkan_RenderPass* renderPass;

    Vulkan_VertexDescriptor* vertexDescriptor = nullptr;

    Vulkan_GraphicsPipelineConfig config;
};

class Vulkan_GraphicsPipeline {
public:
    VkPipeline graphicsPipeline;
    
    Vulkan_PipelineLayout* pipelineLayout;

    Vulkan_GraphicsPipelineCreateInfo createInfo;

    void init(Vulkan_GraphicsPipelineCreateInfo& aCreateInfo);

    void destroy();

    void compile();

    void recompile();

    //void createPipelineLayout();

    void uploadPushConstants(void* data, uint8_t index);

    void bind();

    static Vulkan_PipelineConfigInfo defaultPipelineConfigInfo(Vulkan_GraphicsPipelineConfig& config, Vulkan_GraphicsPipelineCreateInfo& graphicsPipelineCreateInfo);
};

} //namespace lv

#endif
