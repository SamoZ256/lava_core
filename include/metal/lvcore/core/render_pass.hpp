#ifndef LV_METAL_RENDER_PASS_H
#define LV_METAL_RENDER_PASS_H

#include "attachment.hpp"

namespace lv {

class Metal_Subpass {
public:
    std::vector<Metal_SubpassAttachment> colorAttachments;
    Metal_SubpassAttachment depthAttachment;

    void addColorAttachment(Metal_SubpassAttachment attachment) { colorAttachments.push_back(attachment); }

    void setDepthAttachment(Metal_SubpassAttachment attachment) { depthAttachment = attachment; }
};

class Metal_RenderPass {
public:
    std::vector<Metal_Subpass*> subpasses;

    std::vector<Metal_RenderPassAttachment> colorAttachments;
    Metal_RenderPassAttachment depthAttachment;

    std::vector<Metal_RenderPassAttachment*> sortedAttachments;

    void init();

    void destroy() {}

    void addSubpass(Metal_Subpass* subpass) { subpasses.push_back(subpass); }

    void addColorAttachment(Metal_RenderPassAttachment attachment) { colorAttachments.push_back(attachment); }

    void setDepthAttachment(Metal_RenderPassAttachment attachment) { depthAttachment = attachment; }
};

} //namespace lv

#endif
