#ifndef LV_METAL_GRAPHICS_PIPELINE_H
#define LV_METAL_GRAPHICS_PIPELINE_H

#include "shader_module.hpp"
#include "framebuffer.hpp"
#include "vertex_descriptor.hpp"
#include "pipeline_layout.hpp"
#include "buffer.hpp"

namespace lv {

struct OpenGL_GraphicsPipelineConfig {
    LvCullModeFlags cullMode = LV_CULL_MODE_NONE;
    LvBool depthTestEnable = LV_FALSE;
    LvBool depthWriteEnable = LV_TRUE;
    LvCompareOp depthOp = LV_COMPARE_OP_LESS;
};

struct OpenGL_GraphicsPipelineCreateInfo {
    OpenGL_ShaderModule* vertexShaderModule;
    OpenGL_ShaderModule* fragmentShaderModule;
    OpenGL_PipelineLayout* pipelineLayout;
    OpenGL_RenderPass* renderPass;

    OpenGL_VertexDescriptor* vertexDescriptor = nullptr;

    OpenGL_GraphicsPipelineConfig config;
};

struct OpenGL_PushConstantBinding {
    GLuint location;
    OpenGL_Buffer uniformBuffer;
};

class OpenGL_GraphicsPipeline {
public:
    GLenum shaderProgram;

    GLuint** bindings;
    uint16_t** bufferBindings;
    uint16_t** textureBindings;
    std::vector<OpenGL_PushConstantBinding> pushConstantBindings;

    OpenGL_PipelineLayout* pipelineLayout;

    OpenGL_GraphicsPipelineCreateInfo createInfo;

    void init(OpenGL_GraphicsPipelineCreateInfo& aCreateInfo);

    void destroy();

    void compile();

    void recompile();

    void bind();

    void uploadPushConstants(void* data, uint16_t index);
};

} //namespace lv

#endif
