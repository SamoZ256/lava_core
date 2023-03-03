#ifndef LV_OPENGL_FRAMEBUFFER_H
#define LV_OPENGL_FRAMEBUFFER_H

#include <vector>

#include "render_pass.hpp"

namespace lv {

class OpenGL_Framebuffer {
public:
    uint8_t frameCount = 0;

    GLuint framebuffer = 0;

    OpenGL_RenderPass* renderPass;

    std::vector<OpenGL_FramebufferAttachment> colorAttachments;
    OpenGL_FramebufferAttachment depthAttachment;

    std::vector<LvClearValue> clearValues;

    void init(OpenGL_RenderPass* aRenderPass);

    void destroy() { glDeleteFramebuffers(1, &framebuffer); }

    void addColorAttachment(OpenGL_FramebufferAttachment attachment) { colorAttachments.push_back(attachment); }

    void setDepthAttachment(OpenGL_FramebufferAttachment attachment) { depthAttachment = attachment; }

    void bind();

    void unbind() {}
};

} //namespace lv

#endif
