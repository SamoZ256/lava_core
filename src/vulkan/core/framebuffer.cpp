#include "lvcore/core/framebuffer.hpp"

#include "lvcore/core/swap_chain.hpp"

namespace lv {

void Framebuffer::init(RenderPass* aRenderPass, uint16_t aWidth, uint16_t aHeight) {
    if (frameCount == 0) frameCount = g_swapChain->maxFramesInFlight;

    renderPass = aRenderPass;

    width = aWidth;
    height = aHeight;

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

    for (auto& colorAttachment : colorAttachments)
        maxLayerCount = std::max((uint8_t)std::max((int8_t)colorAttachment.image->layerCount, colorAttachment.imageView->layerCount), maxLayerCount);
    if (depthAttachment.image != nullptr)
        maxLayerCount = std::max((uint8_t)std::max((int8_t)depthAttachment.image->layerCount, depthAttachment.imageView->layerCount), maxLayerCount);

    //Framebuffer
    //std::cout << (int)maxLayerCount << std::endl;
    framebuffers.resize(frameCount);
    for (size_t i = 0; i < frameCount; i++) {
        //std::array<VkImageView, 2> attachments = {swapChainImageViews[i], depthImageViews[i]};
        std::vector<VkImageView> imageViews;
        for (auto& colorAttachment : colorAttachments) {
            //uint8_t index = i < attachment->imageView->imageViews.size() ? i : 0;
            imageViews.push_back(colorAttachment.imageView->imageViews[i]);
        }
        if (depthAttachment.image != nullptr) {
            //uint8_t index = i < depthAttachment.imageView->imageViews.size() ? i : 0;
            imageViews.push_back(depthAttachment.imageView->imageViews[i]);
        }

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass->renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(imageViews.size());
        framebufferInfo.pAttachments = imageViews.data();
        framebufferInfo.width = width;
        framebufferInfo.height = height;
        framebufferInfo.layers = maxLayerCount;

        VK_CHECK_RESULT(vkCreateFramebuffer(g_device->device(), &framebufferInfo, nullptr, &framebuffers[i]))
    }

    commandBuffer.frameCount = frameCount;
    commandBuffer.init();

    //Clear values
    clearValues.resize(colorAttachments.size() + (depthAttachment.image == nullptr ? 0 : 1));
    for (auto& colorAttachment : colorAttachments) {
        clearValues[colorAttachment.attachmentIndex].color = {0.0f, 0.0f, 0.0f, 0.0f};
    }
    if (depthAttachment.image != nullptr)
        clearValues[depthAttachment.attachmentIndex].depthStencil = {1.0f, 0};
}

void Framebuffer::destroyToRecreate() {
    for (auto& framebuffer : framebuffers) {
        vkDestroyFramebuffer(g_device->device(), framebuffer, nullptr);
    }
}

void Framebuffer::destroy() {
    commandBuffer.destroy();
    destroyToRecreate();
}

FramebufferAttachmentDescriptions Framebuffer::getAttachmentDescriptions() {
    return {colorAttachments, depthAttachment};
}

void Framebuffer::bind() {
    //g_swapChain->activeRenderPass = &renderPass->renderPass;
    //g_swapChain->activeCommandBuffers = &commandBuffers;
    uint8_t index = g_swapChain->imageIndex;

    //std::cout << (int8_t)index << " : " << commandBuffers.size() << std::endl;
    commandBuffer.bind();

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass->renderPass;
    renderPassInfo.framebuffer = framebuffers[index];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = VkExtent2D{width, height};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer.commandBuffers[index], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void Framebuffer::unbind() {
    uint8_t index = g_swapChain->imageIndex;

    vkCmdEndRenderPass(commandBuffer.commandBuffers[index]);
    commandBuffer.unbind();
}

void Framebuffer::render() {
    commandBuffer.submit();
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
