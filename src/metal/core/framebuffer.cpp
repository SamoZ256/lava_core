#include "metal/lvcore/core/framebuffer.hpp"

#include "metal/lvcore/core/device.hpp"
#include "metal/lvcore/core/swap_chain.hpp"

#include <iostream>

namespace lv {

void Metal_Framebuffer::init(Metal_RenderPass* renderPass) {
    if (frameCount == 0) frameCount = g_metal_swapChain->maxFramesInFlight;
    
    renderPasses.resize(frameCount);
    for (uint8_t i = 0; i < frameCount; i++) {
        renderPasses[i] = MTL::RenderPassDescriptor::alloc()->init();
        uint16_t maxArrayLength = 1;
        for (uint8_t j = 0; j < colorAttachments.size(); j++) {
            Metal_RenderPassAttachment* renderPassAttachment = renderPass->sortedAttachments[colorAttachments[j].index];
            MTL::RenderPassColorAttachmentDescriptor* attachment = renderPasses[i]->colorAttachments()->object(colorAttachments[j].index);
            attachment->setClearColor(MTL::ClearColor::Make(0.0, 0.0, 0.0, 1.0));
            attachment->setLoadAction(renderPassAttachment->loadOp);
            attachment->setStoreAction(renderPassAttachment->storeOp);
            attachment->setTexture(colorAttachments[j].imageView->imageViews[std::min(i, uint8_t(colorAttachments[j].imageView->frameCount - 1))]);
            maxArrayLength = std::max(maxArrayLength, (uint16_t)colorAttachments[j].imageView->layerCount);
        }

        if (depthAttachment.imageView != nullptr) {
            Metal_RenderPassAttachment* renderPassAttachment = renderPass->sortedAttachments[depthAttachment.index];
            MTL::RenderPassDepthAttachmentDescriptor* attachment = renderPasses[i]->depthAttachment();
            attachment->setClearDepth(1.0);
            attachment->setLoadAction(renderPassAttachment->loadOp);
            attachment->setStoreAction(renderPassAttachment->storeOp);
            attachment->setTexture(depthAttachment.imageView->imageViews[std::min(i, uint8_t(depthAttachment.imageView->frameCount - 1))]);
            //renderPass->setDepthAttachment(attachment);
            maxArrayLength = std::max(maxArrayLength, depthAttachment.imageView->image->layerCount);
        }
        //std::cout << (int)maxArrayLength << std::endl;
        renderPasses[i]->setRenderTargetArrayLength(maxArrayLength);

        //commandBuffers[i] = g_metal_device->commandQueue->commandBuffer();
    }

    commandBuffer.frameCount = frameCount;
    commandBuffer.init();
}

void Metal_Framebuffer::destroy() {
    for (uint8_t i = 0; i < frameCount; i++) {
        renderPasses[i]->release();
        //commandBuffers[i]->release();
    }
}

void Metal_Framebuffer::bind() {
    uint8_t index = std::min(g_metal_swapChain->crntFrame, uint8_t(renderPasses.size() - 1));
    commandBuffer.bind();
    encoder = commandBuffer.createRenderCommandEncoder(renderPasses[index]);
    g_metal_swapChain->activeRenderEncoder = encoder;
    g_metal_swapChain->activeRenderPasses = renderPasses;
}

void Metal_Framebuffer::unbind() {
    encoder->endEncoding();
}

void Metal_Framebuffer::render() {
    commandBuffer.submit();
    encoder->release();
}

} //namespace lv
