#ifndef LV_SWAP_CHAIN_H
#define LV_SWAP_CHAIN_H

#include <stdexcept>

#include <QuartzCore/QuartzCore.hpp>

#define LVND_BACKEND_METAL
#include "lvnd/lvnd.h"

#include "framebuffer.hpp"

namespace lv {

class SwapChain {
public:
    Framebuffer framebuffer;
    Image colorAttachment;
    //Image depthAttachment;

    LvndWindow* _window;
    CA::MetalDrawable* drawable;
    dispatch_semaphore_t semaphore;

    MTL::PixelFormat depthFormat = MTL::PixelFormatDepth32Float;

    uint8_t maxFramesInFlight = 2;
    uint8_t crntFrame = 0;

    SwapChain(LvndWindow* window);

    void init(LvndWindow* window);

    void destroy();

    void resize(LvndWindow* window);

    void acquireNextImage();

    void synchronize();

    void renderAndPresent();

    void renderFullscreenTriangle() { activeFramebuffer->encoder->drawPrimitives(MTL::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(3)); }

    uint32_t width() { return _width; }

    uint32_t height() { return _height; }

    Framebuffer* activeFramebuffer;

private:
    uint32_t _width, _height;
};

extern SwapChain* g_swapChain;

} //namespace lv

#endif
