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

    OpenGL_ShaderModule* vertexShaderModule;
    OpenGL_ShaderModule* fragmentShaderModule;
    OpenGL_PipelineLayout* pipelineLayout;
    OpenGL_RenderPass* renderPass;
    uint8_t subpass = 0;
    OpenGL_VertexDescriptor* vertexDescriptor = nullptr;
    OpenGL_GraphicsPipelineConfig config;
    std::vector<OpenGL_ColorBlendAttachment> colorBlendAttachments;

    void init();

    void destroy();

    void compile();

    void recompile();

    void bind();

    void uploadPushConstants(void* data, uint16_t index);

    void addColorBlendAttachment(OpenGL_ColorBlendAttachment attachment) { colorBlendAttachments.push_back(attachment); }
};

} //namespace lv

#endif
