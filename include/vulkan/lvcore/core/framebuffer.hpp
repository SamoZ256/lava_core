#ifndef LV_FRAMEBUFFER_H
#define LV_FRAMEBUFFER_H

#include "render_pass.hpp"
#include "command_buffer.hpp"

namespace lv {

class Framebuffer {
public:
    uint8_t frameCount = 0;

    std::vector<VkFramebuffer> framebuffers;
    CommandBuffer commandBuffer;
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

    void addColorAttachment(Image* image, ImageView* imageView, uint8_t attachmentIndex, VkAttachmentLoadOp loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE, VkAttachmentStoreOp storeOp = VK_ATTACHMENT_STORE_OP_STORE) {
        colorAttachments.push_back({image, imageView, attachmentIndex, loadOp, storeOp});
    }

    void setDepthAttachment(Image* image, ImageView* imageView, uint8_t attachmentIndex, VkAttachmentLoadOp loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE, VkAttachmentStoreOp storeOp = VK_ATTACHMENT_STORE_OP_STORE) {
        depthAttachment = {image, imageView, attachmentIndex, loadOp, storeOp};
    }

    //void resize(uint16_t aWidth, uint16_t aHeight);

    void bind();

    void unbind();

    void render();
};

} // namespace lv

#endif
