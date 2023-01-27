#ifndef LV_GRAPHICS_PIPELINE_H
#define LV_GRAPHICS_PIPELINE_H

#include <cassert>

#include "render_pass.hpp"
#include "descriptor_set.hpp"
#include "vertex_descriptor.hpp"
#include "shader_module.hpp"

namespace lv {

struct PipelineConfigInfo {
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

struct GraphicsPipelineConfig {
    std::vector<unsigned int> blends = {VK_FALSE};
    VkCullModeFlagBits cullMode = VK_CULL_MODE_NONE;
    unsigned int depthTestEnable = VK_FALSE;
    unsigned int depthWriteEnable = VK_TRUE;
    VkCompareOp depthOp = VK_COMPARE_OP_LESS;
};

struct GraphicsPipelineCreateInfo {
    ShaderModule* vertexShaderModule;
    ShaderModule* fragmentShaderModule;
    PipelineLayout* pipelineLayout;
    RenderPass* renderPass;

    VertexDescriptor* vertexDescriptor = nullptr;
    //std::vector<VkPushConstantRange> pushConstantRanges;

    GraphicsPipelineConfig config;
};

class GraphicsPipeline {
public:
    VkPipeline graphicsPipeline;
    //VkPipelineLayout pipelineLayout;
    
    PipelineLayout& pipelineLayout;
    //ShaderModule geometryShaderModule;

    //VkPushConstantRange* pushConstantRanges;
    //uint8_t pushConstantRangeCount;

    //bool geometryShader = false;

    GraphicsPipeline(GraphicsPipelineCreateInfo& createInfo);

    void destroy();

    //void createPipelineLayout();

    void uploadPushConstants(void* data, uint8_t index);

    void bind();

    static PipelineConfigInfo defaultPipelineConfigInfo(GraphicsPipelineConfig& config, uint8_t colorAttachmentCount);
};

} //namespace lv

#endif
