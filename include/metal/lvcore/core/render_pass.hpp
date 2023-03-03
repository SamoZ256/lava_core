#ifndef LV_METAL_RENDER_PASS_H
#define LV_METAL_RENDER_PASS_H

#include "attachment.hpp"

namespace lv {

class Metal_Subpass {
public:
    std::vector<Metal_SubpassAttachment> colorAttachments;
    Metal_SubpassAttachment depthAttachment;
    std::vector<Metal_SubpassAttachment> inputAttachments;

    void addColorAttachment(Metal_SubpassAttachment attachment) { colorAttachments.push_back(attachment); }

    void setDepthAttachment(Metal_SubpassAttachment attachment) { depthAttachment = attachment; }

    void addInputAttachment(Metal_SubpassAttachment attachment) { inputAttachments.push_back(attachment); }
};

class Metal_RenderPass {
public:
    std::vector<Metal_Subpass*> subpasses;

    std::vector<Metal_RenderPassAttachment> attachments;

    std::vector<Metal_RenderPassAttachment*> sortedAttachments;

    void init();

    void destroy() {}

    void addSubpass(Metal_Subpass* subpass) { subpasses.push_back(subpass); }

    void addAttachment(Metal_RenderPassAttachment attachment) { attachments.push_back(attachment); }
};

} //namespace lv

#endif
