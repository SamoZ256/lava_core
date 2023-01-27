#ifndef LV_RENDER_PASS_H
#define LV_RENDER_PASS_H

#include <vector>

#include "attachment.hpp"

namespace lv {

struct FramebufferAttachmentDescriptions {
    std::vector<Attachment>& colorAttachments;
    Attachment& depthAttachment;
};

class RenderPass {
public:
    VkRenderPass renderPass;

    uint8_t colorAttachmentCount;
    bool hasDepthAttachment;

    std::vector<VkSubpassDependency> dependencies;

    void init(FramebufferAttachmentDescriptions framebufferAttachmentDescriptions/*, bool readDepthAttachment = false*/);

    void destroy();
};

} //namespace lv

#endif
