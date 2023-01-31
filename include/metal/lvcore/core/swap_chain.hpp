#ifndef LV_METAL_SWAP_CHAIN_H
#define LV_METAL_SWAP_CHAIN_H

#include <stdexcept>

#include <QuartzCore/QuartzCore.hpp>

#define LVND_BACKEND_METAL
#include "lvnd/lvnd.h"

#include "framebuffer.hpp"

namespace lv {

struct Metal_SwapChainCreateInfo {
	LvndWindow* window;
  	bool vsyncEnabled = true;
	uint8_t maxFramesInFlight = 2;
	bool clearAttachment = false;
};

class Metal_SwapChain {
public:
    uint8_t maxFramesInFlight;
    uint8_t crntFrame = 0;

    Metal_Subpass subpass;
    Metal_RenderPass renderPass;
    Metal_Framebuffer framebuffer;
    Metal_Image colorImage;
    Metal_ImageView colorImageView;
    //Image depthAttachment;

    LvndWindow* _window;
    CA::MetalDrawable* drawable;
    dispatch_semaphore_t semaphore;

    MTL::PixelFormat depthFormat = MTL::PixelFormatDepth32Float;

    Metal_SwapChain(Metal_SwapChainCreateInfo& createInfo);

    void init(LvndWindow* window);

    void destroy();

    void resize(LvndWindow* window);

    void acquireNextImage();

    void synchronize();

    void renderAndPresent();

    MTL::CommandBuffer* getActiveCommandBuffer() { return activeCommandBuffer->commandBuffers[std::min(crntFrame, uint8_t(activeCommandBuffer->commandBuffers.size() - 1))]; }

    void renderFullscreenTriangle() { activeRenderEncoder->drawPrimitives(MTL::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(3)); }

    uint32_t width() { return _width; }

    uint32_t height() { return _height; }

    Metal_CommandBuffer* activeCommandBuffer = nullptr;
    MTL::RenderCommandEncoder* activeRenderEncoder = nullptr;
    MTL::ComputeCommandEncoder* activeComputeEncoder = nullptr;
    std::vector<MTL::RenderPassDescriptor*> activeRenderPasses;

private:
    uint32_t _width, _height;
};

extern Metal_SwapChain* g_metal_swapChain;

} //namespace lv

#endif
