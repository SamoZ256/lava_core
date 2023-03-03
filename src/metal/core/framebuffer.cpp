#include "metal/lvcore/core/framebuffer.hpp"

#include "metal/lvcore/core/device.hpp"
#include "metal/lvcore/core/swap_chain.hpp"

#include <iostream>

namespace lv {

void Metal_Framebuffer::init(Metal_RenderPass* renderPass) {
    if (frameCount == 0) frameCount = g_metal_swapChain->maxFramesInFlight;

    sortedAttachments.resize(colorAttachments.size());
    for (uint8_t i = 0; i < colorAttachments.size(); i++)
        sortedAttachments[colorAttachments[i].index] = &colorAttachments[i];
    
    subpasses.resize(renderPass->subpasses.size());
    for (uint8_t subpassIndex = 0; subpassIndex < subpasses.size(); subpassIndex++) {
        Metal_FramebufferSubpass framebufferSubpass;
        framebufferSubpass.renderPasses.resize(frameCount);
        auto& subpass = renderPass->subpasses[subpassIndex];
        for (uint8_t i = 0; i < frameCount; i++) {
            framebufferSubpass.renderPasses[i] = MTL::RenderPassDescriptor::alloc()->init();
            uint16_t maxArrayLength = 1;
            for (uint8_t j = 0; j < subpass->colorAttachments.size(); j++) {
                Metal_SubpassAttachment& subpassAttachment = subpass->colorAttachments[j];
                Metal_RenderPassAttachment* renderPassAttachment = renderPass->sortedAttachments[subpassAttachment.index];
                Metal_FramebufferAttachment* framebufferAttachment = sortedAttachments[subpassAttachment.index];
                MTL::RenderPassColorAttachmentDescriptor* attachment = framebufferSubpass.renderPasses[i]->colorAttachments()->object(j);
                attachment->setClearColor(MTL::ClearColor::Make(/*colorAttachments[i].clearValue.color.float32[0], colorAttachments[i].clearValue.color.float32[1], colorAttachments[i].clearValue.color.float32[2], colorAttachments[i].clearValue.color.float32[3]*/1.0f, 1.0f, 1.0f, 1.0f)); //TODO: set the real clear value
                attachment->setLoadAction(subpassIndex == 0 ? renderPassAttachment->loadOp : LV_ATTACHMENT_LOAD_OP_LOAD);
                attachment->setStoreAction(subpassIndex == subpasses.size() - 1 ? renderPassAttachment->storeOp : LV_ATTACHMENT_STORE_OP_STORE);
                attachment->setTexture(framebufferAttachment->imageView->imageViews[std::min(i, uint8_t(framebufferAttachment->imageView->frameCount - 1))]);
                maxArrayLength = std::max(maxArrayLength, (uint16_t)framebufferAttachment->imageView->layerCount);
                //std::cout << "Index: " << (int)subpassAttachment.index << std::endl;
            }

            if (subpass->depthAttachment.index != -1) {
                Metal_SubpassAttachment& subpassAttachment = subpass->depthAttachment;
                Metal_RenderPassAttachment* renderPassAttachment = renderPass->sortedAttachments[subpassAttachment.index];
                MTL::RenderPassDepthAttachmentDescriptor* attachment = framebufferSubpass.renderPasses[i]->depthAttachment();
                attachment->setClearDepth(depthAttachment.clearValue.depthStencil.depth);
                attachment->setLoadAction(subpassIndex == 0 ? renderPassAttachment->loadOp : LV_ATTACHMENT_LOAD_OP_LOAD);
                attachment->setStoreAction(subpassIndex == subpasses.size() - 1 ? renderPassAttachment->storeOp : LV_ATTACHMENT_STORE_OP_STORE);
                attachment->setTexture(depthAttachment.imageView->imageViews[std::min(i, uint8_t(depthAttachment.imageView->frameCount - 1))]);
                maxArrayLength = std::max(maxArrayLength, depthAttachment.imageView->layerCount);
            }

            framebufferSubpass.renderPasses[i]->setRenderTargetArrayLength(maxArrayLength);
        }
        subpasses[subpassIndex] = framebufferSubpass;
    }
    std::cout << std::endl;
}

void Metal_Framebuffer::destroy() {
    for (auto& subpass : subpasses) {
        for (uint8_t i = 0; i < frameCount; i++) {
            subpass.renderPasses[i]->release();
        }
        if (subpass.encoder != nullptr) {
            subpass.encoder->release();
            subpass.encoder = nullptr;
        }
    }
}

void Metal_Framebuffer::bind() {
    crntSubpass = 0;
    _beginSubpass();
}

void Metal_Framebuffer::unbind() {
    _endSubpass();
}

void Metal_Framebuffer::nextSubpass() {
    _endSubpass();
    crntSubpass++;
    _beginSubpass();
}

void Metal_Framebuffer::_beginSubpass() {
    uint8_t index = std::min(g_metal_swapChain->crntFrame, uint8_t(frameCount - 1));
    if (subpasses[crntSubpass].encoder != nullptr)
        subpasses[crntSubpass].encoder->release();
    subpasses[crntSubpass].encoder = g_metal_swapChain->getActiveCommandBuffer()->renderCommandEncoder(subpasses[crntSubpass].renderPasses[index]);

    g_metal_swapChain->activeRenderEncoder = subpasses[crntSubpass].encoder;
    g_metal_swapChain->activeRenderPass = subpasses[crntSubpass].renderPasses[index];
}

void Metal_Framebuffer::_endSubpass() {
    subpasses[crntSubpass].encoder->endEncoding();
}

} //namespace lv
