#ifndef LV_VULKAN_RENDER_PASS_H
#define LV_VULKAN_RENDER_PASS_H

#include <vector>

#include "attachment.hpp"

namespace lv {

class Vulkan_Subpass {
public:
    std::vector<Vulkan_SubpassAttachment> colorAttachments;
    Vulkan_SubpassAttachment depthAttachment;
    std::vector<Vulkan_SubpassAttachment> inputAttachments;

    void addColorAttachment(Vulkan_SubpassAttachment attachment) { colorAttachments.push_back(attachment); }

    void setDepthAttachment(Vulkan_SubpassAttachment attachment) { depthAttachment = attachment; }

    void addInputAttachment(Vulkan_SubpassAttachment attachment) { inputAttachments.push_back(attachment); }
};

class Vulkan_RenderPass {
public:
    VkRenderPass renderPass;

    std::vector<Vulkan_Subpass*> subpasses;
    std::vector<VkSubpassDependency> dependencies;

    std::vector<Vulkan_RenderPassAttachment> colorAttachments;
    Vulkan_RenderPassAttachment depthAttachment;
    std::vector<Vulkan_RenderPassAttachment> inputAttachments;

    void init();

    void destroy();

    void addSubpass(Vulkan_Subpass* subpass) { subpasses.push_back(subpass); }

    void addColorAttachment(Vulkan_RenderPassAttachment attachment) { colorAttachments.push_back(attachment); }

    void setDepthAttachment(Vulkan_RenderPassAttachment attachment) { depthAttachment = attachment; }

    void addInputAttachment(Vulkan_RenderPassAttachment attachment) { inputAttachments.push_back(attachment); }
};

} //namespace lv

#endif
