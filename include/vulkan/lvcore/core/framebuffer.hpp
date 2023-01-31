#ifndef LV_VULKAN_FRAMEBUFFER_H
#define LV_VULKAN_FRAMEBUFFER_H

#include "render_pass.hpp"
#include "command_buffer.hpp"

namespace lv {

class Vulkan_Framebuffer {
public:
    uint8_t frameCount = 0;

    std::vector<VkFramebuffer> framebuffers;
    Vulkan_CommandBuffer commandBuffer;
    Vulkan_RenderPass* renderPass;

    std::vector<Vulkan_FramebufferAttachment> colorAttachments;
    Vulkan_FramebufferAttachment depthAttachment;

    uint16_t width = 0, height = 0;
    uint16_t maxLayerCount = 1;

    std::vector<VkClearValue> clearValues{};

    void init(Vulkan_RenderPass* aRenderPass);

    void destroy();

    void destroyToRecreate();

    void addColorAttachment(Vulkan_FramebufferAttachment attachment) {
        colorAttachments.push_back(attachment);
    }

    void setDepthAttachment(Vulkan_FramebufferAttachment attachment) {
        depthAttachment = attachment;
    }

    //void resize(uint16_t aWidth, uint16_t aHeight);

    void bind();

    void unbind();

    void render();
};

} // namespace lv

#endif
