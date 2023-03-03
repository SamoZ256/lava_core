#include "vulkan/lvcore/core/framebuffer.hpp"

#include <cmath>

#include "vulkan/lvcore/core/swap_chain.hpp"

namespace lv {

void Vulkan_Framebuffer::init(Vulkan_RenderPass* aRenderPass) {
    if (frameCount == 0) frameCount = g_vulkan_swapChain->maxFramesInFlight;

    renderPass = aRenderPass;

    /*
    bool hasDepthAttachment = (depthAttachment.attachmentIndex != -1);
    for (auto& attachment : colorAttachments) {
        if (attachment.image == nullptr)
            throw std::invalid_argument("Color attachment number " + std::to_string(attachment.attachmentIndex) + " has invalid image");
        if (attachment.imageView == nullptr)
            throw std::invalid_argument("Color attachment number " + std::to_string(attachment.attachmentIndex) + " has invalid image view");
        if (attachment.attachmentIndex == -1)
            throw std::invalid_argument("Color attachment has invalid attachment index");
    }
    if (hasDepthAttachment) {
        if (depthAttachment.image == nullptr)
            throw std::invalid_argument("Depth attachment has invalid image");
        if (depthAttachment.imageView == nullptr)
            throw std::invalid_argument("Depth attachment has invalid image view");
        if (depthAttachment.attachmentIndex == -1)
            throw std::invalid_argument("Depth attachment has invalid attachment index");
    }
    */

    for (auto& colorAttachment : colorAttachments) {
        maxLayerCount = std::max((uint16_t)colorAttachment.imageView->layerCount, maxLayerCount);
        uint16_t scale = pow(2, colorAttachment.imageView->baseMip);
        width = colorAttachment.imageView->image->width / scale;
        height = colorAttachment.imageView->image->height / scale;
        //std::cout << (int)colorAttachment.imageView->image->width << " : " << (int)width << std::endl;
    }
    if (depthAttachment.index != -1) {
        maxLayerCount = std::max((uint16_t)depthAttachment.imageView->layerCount, maxLayerCount);
        uint16_t scale = pow(2, depthAttachment.imageView->baseMip);
        width = depthAttachment.imageView->image->width / scale;
        height = depthAttachment.imageView->image->height / scale;
    }

    //Framebuffer
    framebuffers.resize(frameCount);
    for (size_t i = 0; i < frameCount; i++) {
        std::vector<VkImageView> imageViews;
        for (auto& colorAttachment : colorAttachments)
            imageViews.push_back(colorAttachment.imageView->imageViews[i]);
        if (depthAttachment.index != -1)
            imageViews.push_back(depthAttachment.imageView->imageViews[i]);

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass->renderPass;
        framebufferInfo.attachmentCount = imageViews.size();
        framebufferInfo.pAttachments = imageViews.data();
        framebufferInfo.width = width;
        framebufferInfo.height = height;
        framebufferInfo.layers = maxLayerCount;

        VK_CHECK_RESULT(vkCreateFramebuffer(g_vulkan_device->device(), &framebufferInfo, nullptr, &framebuffers[i]))
    }

    //Clear values
    clearValues.resize(colorAttachments.size() + (depthAttachment.index == -1 ? 0 : 1));
    for (auto& colorAttachment : colorAttachments)
        clearValues[colorAttachment.index].color = {colorAttachment.clearValue.color.float32[0], colorAttachment.clearValue.color.float32[1], colorAttachment.clearValue.color.float32[2], colorAttachment.clearValue.color.float32[3]};

    if (depthAttachment.index != -1)
        clearValues[depthAttachment.index].depthStencil = {depthAttachment.clearValue.depthStencil.depth, depthAttachment.clearValue.depthStencil.stencil};
}

void Vulkan_Framebuffer::destroy() {
    for (auto& framebuffer : framebuffers) {
        vkDestroyFramebuffer(g_vulkan_device->device(), framebuffer, nullptr);
    }
}

void Vulkan_Framebuffer::bind() {
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass->renderPass;
    renderPassInfo.framebuffer = framebuffers[g_vulkan_swapChain->imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = VkExtent2D{width, height};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(g_vulkan_swapChain->getActiveCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void Vulkan_Framebuffer::unbind() {
    uint8_t index = g_vulkan_swapChain->imageIndex;

    vkCmdEndRenderPass(g_vulkan_swapChain->getActiveCommandBuffer());
}

void Vulkan_Framebuffer::nextSubpass() {
    vkCmdNextSubpass(g_vulkan_swapChain->getActiveCommandBuffer(), VK_SUBPASS_CONTENTS_INLINE);
}

/*
void Framebuffer::resize(uint16_t aWidth, uint16_t aHeight) {
    for (auto& attachment : colorAttachments) {
        if (!attachment->image->resized) {
            attachment.image->resize(aWidth, aHeight);
            //std::cout << attachment.image->resized << std::endl;
            attachment.image->resized = true;
            //std::cout << attachment.image->resized << std::endl;
        }
        attachment.imageView->init(attachment.image);
    }
    for (auto& attachment : colorAttachments) {
        attachment.image->resized = false;
    }
    if (depthAttachment.attachmentIndex != -1) depthAttachment.image->resize(aWidth, aHeight);
    destroyToRecreate();
    renderPass->init(getAttachmentDescriptions());
    init(renderPass, aWidth, aHeight);
}
*/

} //namespace lv
