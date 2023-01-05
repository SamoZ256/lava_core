#include "lvcore/core/framebuffer.hpp"

#include "lvcore/core/device.hpp"
#include "lvcore/core/swap_chain.hpp"

#include <iostream>

namespace lv {

void Framebuffer::init() {
    if (frameCount == -1) frameCount = g_swapChain->maxFramesInFlight;
    
    renderPasses.resize(frameCount);
    commandBuffers.resize(frameCount);
    for (uint8_t i = 0; i < frameCount; i++) {
        renderPasses[i] = MTL::RenderPassDescriptor::alloc()->init();
        uint16_t maxArrayLength = 1;
        for (auto& colorAttachment : colorAttachments) {
            MTL::RenderPassColorAttachmentDescriptor* attachment = renderPasses[i]->colorAttachments()->object(colorAttachment.attachmentIndex);
            attachment->setClearColor(MTL::ClearColor::Make(0.0, 0.0, 0.0, 1.0));
            attachment->setLoadAction(colorAttachment.image->loadOp);
            attachment->setStoreAction(MTL::StoreActionStore);
            attachment->setTexture(colorAttachment.image->images[i]);
            maxArrayLength = std::max(maxArrayLength, colorAttachment.image->layerCount);
        }

        if (depthAttachment.attachmentIndex != -1) {
            MTL::RenderPassDepthAttachmentDescriptor* attachment = renderPasses[i]->depthAttachment();
            attachment->setClearDepth(1.0);
            attachment->setLoadAction(depthAttachment.image->loadOp);
            attachment->setStoreAction(MTL::StoreActionStore);
            attachment->setTexture(depthAttachment.image->images[i]);
            //renderPass->setDepthAttachment(attachment);
            maxArrayLength = std::max(maxArrayLength, depthAttachment.image->layerCount);
        }
        //std::cout << (int)maxArrayLength << std::endl;
        renderPasses[i]->setRenderTargetArrayLength(maxArrayLength);

        commandBuffers[i] = g_device->commandQueue->commandBuffer();
    }
}

void Framebuffer::destroy() {
    for (uint8_t i = 0; i < frameCount; i++) {
        renderPasses[i]->release();
        commandBuffers[i]->release();
    }
}

void Framebuffer::bind() {
    uint8_t index = std::min(g_swapChain->crntFrame, uint8_t(renderPasses.size() - 1));
    commandBuffers[index]->release();
    commandBuffers[index] = g_device->commandQueue->commandBuffer();
    g_swapChain->activeFramebuffer = this;
    encoder = commandBuffers[index]->renderCommandEncoder(renderPasses[index]);
}

void Framebuffer::unbind() {
    encoder->endEncoding();
}

void Framebuffer::render() {
    uint8_t index = std::min(g_swapChain->crntFrame, uint8_t(renderPasses.size() - 1));
    commandBuffers[index]->commit();
    encoder->release();
}

} //namespace lv
