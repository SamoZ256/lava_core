#include "metal/lvcore/core/command_buffer.hpp"

#include "metal/lvcore/core/device.hpp"
#include "metal/lvcore/core/swap_chain.hpp"

namespace lv {

void Metal_CommandBuffer::init() {
    if (frameCount == 0) frameCount = g_metal_swapChain->maxFramesInFlight;

    commandBuffers.resize(frameCount);
}

void Metal_CommandBuffer::bind() {
    uint8_t index = std::min(g_metal_swapChain->crntFrame, uint8_t(frameCount - 1));
    commandBuffers[index] = g_metal_device->commandQueue->commandBuffer();
    g_metal_swapChain->activeCommandBuffer = this;
}

void Metal_CommandBuffer::submit() {
    uint8_t index = std::min(g_metal_swapChain->crntFrame, uint8_t(frameCount - 1));
    commandBuffers[index]->commit();
    commandBuffers[index]->release();
}

MTL::RenderCommandEncoder* Metal_CommandBuffer::createRenderCommandEncoder(MTL::RenderPassDescriptor* renderPass) {
    uint8_t index = std::min(g_metal_swapChain->crntFrame, uint8_t(frameCount - 1));
    return commandBuffers[index]->renderCommandEncoder(renderPass);
}

MTL::ComputeCommandEncoder* Metal_CommandBuffer::createComputeCommandEncoder() {
    uint8_t index = std::min(g_metal_swapChain->crntFrame, uint8_t(frameCount - 1));
    return commandBuffers[index]->computeCommandEncoder();
}

} //namespace lv
