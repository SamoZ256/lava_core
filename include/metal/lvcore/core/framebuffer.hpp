#ifndef LV_FRAMEBUFFER_H
#define LV_FRAMEBUFFER_H

#include <vector>

#include "attachment.hpp"

namespace lv {

class Framebuffer {
public:
    int8_t frameCount = -1;

    std::vector<MTL::RenderPassDescriptor*> renderPasses;
    std::vector<MTL::CommandBuffer*> commandBuffers;
    MTL::RenderCommandEncoder* encoder = nullptr;

    std::vector<Attachment> colorAttachments;
    Attachment depthAttachment;

    void init();

    void destroy();

    void addColorAttachment(Attachment attachment) { colorAttachments.push_back(attachment); }

    void setDepthAttachment(Attachment attachment) { depthAttachment = attachment; }

    void bind();

    void unbind();

    void render();
};

} //namespace lv

#endif
