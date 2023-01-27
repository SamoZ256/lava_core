#ifndef LV_GRAPHICS_PIPELINE_H
#define LV_GRAPHICS_PIPELINE_H

#include "shader_module.hpp"
#include "framebuffer.hpp"
#include "vertex_descriptor.hpp"

namespace lv {

struct GraphicsPipelineConfig {
    std::vector<bool> blends = {false};
    MTL::CullMode cullMode = MTL::CullModeNone;
    bool depthTestEnable = false;
    bool depthWriteEnable = true;
    MTL::CompareFunction depthOp = MTL::CompareFunctionLess;
};

struct GraphicsPipelineCreateInfo {
    ShaderModule* vertexShaderModule;
    ShaderModule* fragmentShaderModule;
    Framebuffer* framebuffer = nullptr;

    VertexDescriptor* vertexDescriptor = nullptr;

    GraphicsPipelineConfig config;
};

class GraphicsPipeline {
public:
    MTL::RenderPipelineState* graphicsPipeline;

    MTL::DepthStencilState* depthStencilState;
    MTL::CullMode cullMode;

    GraphicsPipeline(GraphicsPipelineCreateInfo& createInfo);

    void destroy() { graphicsPipeline->release(); }

    void bind();

    void uploadPushConstants(void* data, uint16_t index, size_t size, LvShaderStage shaderStage);
};

} //namespace lv

#endif
