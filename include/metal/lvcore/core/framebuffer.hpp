#ifndef LV_METAL_FRAMEBUFFER_H
#define LV_METAL_FRAMEBUFFER_H

#include <vector>

#include "render_pass.hpp"
#include "command_buffer.hpp"

namespace lv {

struct Metal_FramebufferSubpass {
    std::vector<MTL::RenderPassDescriptor*> renderPasses;
    MTL::RenderCommandEncoder* encoder = nullptr;
};

class Metal_Framebuffer {
public:
    uint8_t frameCount = 0;

    uint8_t crntSubpass = 0;

    std::vector<Metal_FramebufferSubpass> subpasses;

    std::vector<Metal_FramebufferAttachment> colorAttachments;
    Metal_FramebufferAttachment depthAttachment;
    std::vector<Metal_FramebufferAttachment*> sortedAttachments;

    void init(Metal_RenderPass* renderPass);

    void destroy();

    void addColorAttachment(Metal_FramebufferAttachment attachment) { colorAttachments.push_back(attachment); }

    void setDepthAttachment(Metal_FramebufferAttachment attachment) { depthAttachment = attachment; }

    void bind();

    void unbind();

    void nextSubpass();

    void _beginSubpass();

    void _endSubpass();
};

} //namespace lv

#endif
