#ifndef LV_METAL_FRAMEBUFFER_H
#define LV_METAL_FRAMEBUFFER_H

#include <vector>

#include "render_pass.hpp"
#include "command_buffer.hpp"

namespace lv {

class Metal_Framebuffer {
public:
    uint8_t frameCount = 0;

    std::vector<MTL::RenderPassDescriptor*> renderPasses;
    MTL::RenderCommandEncoder* encoder = nullptr;

    std::vector<Metal_FramebufferAttachment> colorAttachments;
    Metal_FramebufferAttachment depthAttachment;
    std::vector<Metal_FramebufferAttachment> inputAttachments;

    void init(Metal_RenderPass* renderPass);

    void destroy();

    void addColorAttachment(Metal_FramebufferAttachment attachment) { colorAttachments.push_back(attachment); }

    void setDepthAttachment(Metal_FramebufferAttachment attachment) { depthAttachment = attachment; }

    void addInputAttachment(Metal_FramebufferAttachment attachment) { inputAttachments.push_back(attachment); }

    void bind();

    void unbind();
};

} //namespace lv

#endif
