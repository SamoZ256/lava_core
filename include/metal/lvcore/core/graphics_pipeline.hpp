#ifndef LV_METAL_GRAPHICS_PIPELINE_H
#define LV_METAL_GRAPHICS_PIPELINE_H

#include "shader_module.hpp"
#include "render_pass.hpp"
#include "vertex_descriptor.hpp"
#include "pipeline_layout.hpp"

namespace lv {

struct Metal_GraphicsPipelineConfig {
    MTL::CullMode cullMode = MTL::CullModeNone;
    bool depthTestEnable = false;
    bool depthWriteEnable = true;
    MTL::CompareFunction depthOp = MTL::CompareFunctionLess;
};

class Metal_GraphicsPipeline {
public:
    MTL::RenderPipelineState* graphicsPipeline;

    MTL::DepthStencilState* depthStencilState;

    Metal_ShaderModule* vertexShaderModule;
    Metal_ShaderModule* fragmentShaderModule;
    Metal_PipelineLayout* pipelineLayout;
    Metal_RenderPass* renderPass;
    uint8_t subpassIndex = 0;
    Metal_VertexDescriptor* vertexDescriptor = nullptr;
    Metal_GraphicsPipelineConfig config;
    std::vector<Metal_ColorBlendAttachment> colorBlendAttachments;

    void init();

    void destroy() { graphicsPipeline->release(); }

    void compile();

    void recompile();

    void bind();

    void uploadPushConstants(void* data, uint16_t index/*, size_t size, LvShaderStageFlags shaderStage*/);

    void addColorBlendAttachment(Metal_ColorBlendAttachment attachment) { colorBlendAttachments.push_back(attachment); }
};

} //namespace lv

#endif
