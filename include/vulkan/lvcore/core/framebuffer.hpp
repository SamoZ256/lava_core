#ifndef LV_VULKAN_FRAMEBUFFER_H
#define LV_VULKAN_FRAMEBUFFER_H

#include "render_pass.hpp"
#include "command_buffer.hpp"

namespace lv {

class Vulkan_Framebuffer {
public:
    uint8_t frameCount = 0;

    std::vector<VkFramebuffer> framebuffers;
    Vulkan_RenderPass* renderPass;

    std::vector<Vulkan_FramebufferAttachment> colorAttachments;
    Vulkan_FramebufferAttachment depthAttachment;

    uint16_t width = 0, height = 0;
    uint16_t maxLayerCount = 1;

    std::vector<VkClearValue> clearValues{};

    void init(Vulkan_RenderPass* aRenderPass);

    void destroy();

    void addColorAttachment(Vulkan_FramebufferAttachment attachment) { colorAttachments.push_back(attachment); }

    void setDepthAttachment(Vulkan_FramebufferAttachment attachment) { depthAttachment = attachment; }

    void bind();

    void unbind();

    void nextSubpass();
};

} // namespace lv

#endif
