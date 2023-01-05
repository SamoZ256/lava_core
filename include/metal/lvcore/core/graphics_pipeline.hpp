#ifndef LV_GRAPHICS_PIPELINE_H
#define LV_GRAPHICS_PIPELINE_H

#include "enums.hpp"

#include "swap_chain.hpp"
#include "framebuffer.hpp"
#include "vertex_descriptor.hpp"

namespace lv {

dispatch_data_t convertStringToDispatchData(std::string str);

struct SpecializationMapEntry {
    uint32_t constantID;
    void* data;
    MTL::DataType dataType;
};

struct ShaderModuleCreateInfo {
    const char* filename;
    const char* functionName = "main0";
    std::vector<SpecializationMapEntry> specializationConstants;
};

class ShaderModule {
public:
    MTL::Library* library;
    MTL::Function* function;

    ShaderModule(ShaderModuleCreateInfo& createInfo);

    void destroy();
};

struct GraphicsPipelineConfig {
    std::vector<bool> blends = {false};
    MTL::CullMode cullMode = MTL::CullModeNone;
    bool depthTest = false;
    bool depthWrite = true;
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