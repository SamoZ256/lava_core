#include "lvcore/core/render_pass.hpp"

#include "lvcore/core/common.hpp"

#include "lvcore/core/swap_chain.hpp"

namespace lv {

void RenderPass::init(FramebufferAttachmentDescriptions framebufferAttachmentDescriptions/*, bool readDepthAttachment*/) {
    colorAttachmentCount = framebufferAttachmentDescriptions.colorAttachments.size();
    hasDepthAttachment = framebufferAttachmentDescriptions.hasDepthAttachment;

    std::vector<VkAttachmentDescription> colorAttachmentDecriptions;
    std::vector<VkAttachmentReference> colorAttachmentReferences;

    for (int i = 0; i < framebufferAttachmentDescriptions.colorAttachments.size(); i++) {
        //colorAttachments[i]->usage |= VK_IMAGE_USAGE_SAMPLED_BIT;
        colorAttachmentDecriptions.push_back(framebufferAttachmentDescriptions.colorAttachments[i].image->getAttachmentDescription(/*framebufferAttachmentDescriptions.colorAttachments[i].image->crntLayout*/));
        //framebufferAttachmentDescriptions.colorAttachments[i].image->crntLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        colorAttachmentReferences.push_back(framebufferAttachmentDescriptions.colorAttachments[i].image->getAttachmentReference(framebufferAttachmentDescriptions.colorAttachments[i].attachmentIndex, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL/*, framebufferAttachmentDescriptions.colorAttachments[i].image->crntLayout*/));
    }

    VkAttachmentDescription depthAttachmentDecription;
    VkAttachmentReference depthAttachmentReference;
    if (framebufferAttachmentDescriptions.hasDepthAttachment) {
        //if (readDepthAttachment) framebufferAttachmentDescriptions.depthAttachment.image->usage |= VK_IMAGE_USAGE_SAMPLED_BIT;
        depthAttachmentDecription = framebufferAttachmentDescriptions.depthAttachment.image->getAttachmentDescription(/*framebufferAttachmentDescriptions.depthAttachment.image->crntLayout*/);
        //framebufferAttachmentDescriptions.depthAttachment.image->crntLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        depthAttachmentReference = framebufferAttachmentDescriptions.depthAttachment.image->getAttachmentReference(framebufferAttachmentDescriptions.depthAttachment.attachmentIndex, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL/*, framebufferAttachmentDescriptions.depthAttachment.image->crntLayout*/);
    }

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = framebufferAttachmentDescriptions.colorAttachments.size();
    subpass.pColorAttachments = colorAttachmentReferences.data();
    subpass.pDepthStencilAttachment = framebufferAttachmentDescriptions.hasDepthAttachment ? &depthAttachmentReference : nullptr;

    /*
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.srcAccessMask = 0;
    dependency.srcStageMask =
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstSubpass = 0;
    dependency.dstStageMask =
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask =
        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    */
    
    /*
    dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass = 0;
    dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    dependencies[1].srcSubpass = 0;
    dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
    */

    std::vector<VkAttachmentDescription> attachmentDescriptions = colorAttachmentDecriptions;
    if (framebufferAttachmentDescriptions.hasDepthAttachment) attachmentDescriptions.push_back(depthAttachmentDecription);

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachmentDescriptions.size());
    renderPassInfo.pAttachments = attachmentDescriptions.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = dependencies.size();
    renderPassInfo.pDependencies = dependencies.data();

    VK_CHECK_RESULT(vkCreateRenderPass(g_device->device(), &renderPassInfo, nullptr, &renderPass))
}

void RenderPass::destroy() {
    vkDestroyRenderPass(g_device->device(), renderPass, nullptr);
}

} //namespace lv
