#include "metal/lvcore/core/swap_chain.hpp"

#include "metal/lvcore/core/device.hpp"

#include <iostream>

namespace lv {

Metal_SwapChain* g_metal_swapChain = nullptr;

void Metal_SwapChainFramebuffer::bind() {
    encoder = g_metal_swapChain->commandBuffer.commandBuffers[g_metal_swapChain->crntFrame]->renderCommandEncoder(renderPass);

    g_metal_swapChain->activeRenderPass = renderPass;
    g_metal_swapChain->activeRenderEncoder = encoder;
}

void Metal_SwapChainFramebuffer::unbind() {
    encoder->endEncoding();
    renderPass->release();
    encoder->release();
}

Metal_SwapChain::Metal_SwapChain(Metal_SwapChainCreateInfo& createInfo) {
    g_metal_swapChain = this;

    maxFramesInFlight = createInfo.maxFramesInFlight;
    clearAttachment = createInfo.clearAttachment;

    commandBuffer.init();

    init(createInfo.window);

    semaphore.init();
}

void Metal_SwapChain::init(LvndWindow* window) {
    float xscale, yscale;
    lvndWindowGetFramebufferScale(window, &xscale, &yscale);

    uint16_t width, height;
    lvndWindowGetFramebufferSize(window, &width, &height);

    //window.width = width / xscale;// * xscale;
    //window.height = height / yscale;// * yscale;
    //std::cout << xscale << ", " << yscale << " : " << width << ", " << height << std::endl;
    //std::cout << (int)window.width << ", " << (int)window.height << std::endl;

    // The layer requires the scaled window size in order to properly allocate a drawable
    // texture. If the scaling is not done, we get a "allocation failed" error message
    // from calling [CAMetalLayer nextDrawable].
    lvndMetalCreateLayer(window, width, height, g_metal_device->device);

    _width = width;
    _height = height;

    _window = window;

    //Depth attachment
    /*
    depthAttachment.frameCount = 1;
    depthAttachment.format = MTL::PixelFormatDepth32Float;
    depthAttachment.usage = MTL::TextureUsageRenderTarget;
    depthAttachment.storageMode = MTL::StorageModePrivate;
    depthAttachment.init(width * xscale, height * yscale);
    */
}

void Metal_SwapChain::destroy() {
    semaphore.destroy();
}

void Metal_SwapChain::resize(LvndWindow* window) {
    //depthAttachment.destroy();
    init(window);
}

void Metal_SwapChain::acquireNextImage() {
    drawable = reinterpret_cast<CA::MetalDrawable*>(lvndMetalNextDrawable(_window));
    if (drawable == nullptr) {
        throw std::runtime_error("Failed to acquire drawable");
    }

    framebuffer.renderPass = MTL::RenderPassDescriptor::alloc()->init();
    MTL::RenderPassColorAttachmentDescriptor* attachment = framebuffer.renderPass->colorAttachments()->object(0);
    attachment->setClearColor(MTL::ClearColor::Make(0.0f, 0.0f, 0.0f, 1.0f));
    attachment->setLoadAction(clearAttachment ? LV_ATTACHMENT_LOAD_OP_CLEAR : LV_ATTACHMENT_LOAD_OP_DONT_CARE);
    attachment->setStoreAction(LV_ATTACHMENT_STORE_OP_STORE);
    attachment->setTexture(drawable->texture());
}

void Metal_SwapChain::renderAndPresent() {
    dispatch_semaphore_wait(semaphore.semaphore, DISPATCH_TIME_FOREVER);
    commandBuffer.commandBuffers[crntFrame]->addCompletedHandler(^(MTL::CommandBuffer* cmd) {
        dispatch_semaphore_signal(g_metal_swapChain->semaphore.semaphore);
    });

    commandBuffer.commandBuffers[crntFrame]->presentDrawable(drawable);
    commandBuffer.submit();

    crntFrame = (crntFrame + 1) % maxFramesInFlight;
}

} //namespace lv
