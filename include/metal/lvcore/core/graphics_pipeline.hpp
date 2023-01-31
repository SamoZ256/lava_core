#ifndef LV_METAL_GRAPHICS_PIPELINE_H
#define LV_METAL_GRAPHICS_PIPELINE_H

#include "shader_module.hpp"
#include "framebuffer.hpp"
#include "vertex_descriptor.hpp"

namespace lv {

struct Metal_GraphicsPipelineConfig {
    MTL::CullMode cullMode = MTL::CullModeNone;
    bool depthTestEnable = false;
    bool depthWriteEnable = true;
    MTL::CompareFunction depthOp = MTL::CompareFunctionLess;
};

struct Metal_GraphicsPipelineCreateInfo {
    Metal_ShaderModule* vertexShaderModule;
    Metal_ShaderModule* fragmentShaderModule;
    Metal_RenderPass* renderPass = nullptr;

    Metal_VertexDescriptor* vertexDescriptor = nullptr;

    Metal_GraphicsPipelineConfig config;
};

class Metal_GraphicsPipeline {
public:
    MTL::RenderPipelineState* graphicsPipeline;

    MTL::DepthStencilState* depthStencilState;
    MTL::CullMode cullMode;

    Metal_GraphicsPipeline(Metal_GraphicsPipelineCreateInfo& createInfo);

    void destroy() { graphicsPipeline->release(); }

    void bind();

    void uploadPushConstants(void* data, uint16_t index, size_t size, LvShaderStage shaderStage);
};

} //namespace lv

#endif
