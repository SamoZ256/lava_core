#ifndef LV_OPENGL_RENDER_PASS_H
#define LV_OPENGL_RENDER_PASS_H

#include "attachment.hpp"

namespace lv {

class OpenGL_Subpass {
public:
    std::vector<OpenGL_SubpassAttachment> colorAttachments;
    OpenGL_SubpassAttachment depthAttachment;
    std::vector<OpenGL_SubpassAttachment> inputAttachments;

    void addColorAttachment(OpenGL_SubpassAttachment attachment) { colorAttachments.push_back(attachment); }

    void setDepthAttachment(OpenGL_SubpassAttachment attachment) { depthAttachment = attachment; }

    void addInputAttachment(OpenGL_SubpassAttachment attachment) { inputAttachments.push_back(attachment); }
};

class OpenGL_RenderPass {
public:
    std::vector<OpenGL_Subpass*> subpasses;

    std::vector<OpenGL_RenderPassAttachment> attachments;

    std::vector<OpenGL_RenderPassAttachment*> sortedAttachments;

    void init();

    void destroy() {}

    void addSubpass(OpenGL_Subpass* subpass) { subpasses.push_back(subpass); }

    void addAttachment(OpenGL_RenderPassAttachment attachment) { attachments.push_back(attachment); }
};

} //namespace lv

#endif
