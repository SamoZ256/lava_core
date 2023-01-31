#include "metal/lvcore/core/swap_chain.hpp"

#include "metal/lvcore/core/device.hpp"

#include <iostream>

namespace lv {

Metal_SwapChain* g_metal_swapChain = nullptr;

Metal_SwapChain::Metal_SwapChain(Metal_SwapChainCreateInfo& createInfo) {
    maxFramesInFlight = createInfo.maxFramesInFlight;

    framebuffer.frameCount = 1;

    colorImage.frameCount = 1;
    colorImage.format = MTL::PixelFormatBGRA8Unorm_sRGB;
    colorImage.images.resize(1);

    colorImageView.layerCount = 1;
    colorImageView.mipCount = 1;

    subpass.addColorAttachment({
        .index = 0
    });

    renderPass.addColorAttachment({
        .index = 0
    });

    renderPass.init();

    framebuffer.addColorAttachment({
        .imageView = &colorImageView,
        .index = 0
    });
    //framebuffer.setDepthAttachment({&depthAttachment, 1});

    init(createInfo.window);

    semaphore = dispatch_semaphore_create(maxFramesInFlight);

    g_metal_swapChain = this;
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
    colorImage.destroy();
    framebuffer.destroy();
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

    colorImage.images[0] = drawable->texture();
    colorImageView.init(&colorImage);

    framebuffer.init(&renderPass);
}

void Metal_SwapChain::synchronize() {
    dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
    framebuffer.commandBuffer.commandBuffers[0]->addCompletedHandler(^(MTL::CommandBuffer* cmd) {
        dispatch_semaphore_signal(g_metal_swapChain->semaphore);
    });
}

void Metal_SwapChain::renderAndPresent() {
    framebuffer.commandBuffer.commandBuffers[0]->presentDrawable(drawable);
    framebuffer.render();
    framebuffer.destroy();

    crntFrame = (crntFrame + 1) % maxFramesInFlight;
}

} //namespace lv
