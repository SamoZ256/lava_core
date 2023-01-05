#ifndef LV_FRAMEBUFFER_H
#define LV_FRAMEBUFFER_H

#include "render_pass.hpp"

namespace lv {

class Framebuffer {
public:
    uint8_t frameCount = MAX_FRAMES_IN_FLIGHT;

    std::vector<VkFramebuffer> framebuffers;
    std::vector<VkCommandBuffer> commandBuffers;
    RenderPass* renderPass;

    std::vector<Attachment> colorAttachments;
    Attachment depthAttachment;

    uint16_t width = 0, height = 0;
    uint8_t maxLayerCount = 1;

    std::vector<VkClearValue> clearValues{};

    void init(RenderPass* aRenderPass, uint16_t aWidth, uint16_t aHeight);

    void destroy();

    void destroyToRecreate();

    FramebufferAttachmentDescriptions getAttachmentDescriptions();

    void addColorAttachment(Attachment attachment);

    void setDepthAttachment(Attachment attachment);

    void resize(uint16_t aWidth, uint16_t aHeight);

    void bind();

    void unbind();

    void render();
};

} // namespace lv

#endif
