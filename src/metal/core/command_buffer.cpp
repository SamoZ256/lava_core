#include "metal/lvcore/core/command_buffer.hpp"

#include "metal/lvcore/core/device.hpp"
#include "metal/lvcore/core/swap_chain.hpp"

namespace lv {

void Metal_CommandBuffer::init() {
    if (frameCount == 0) frameCount = g_metal_swapChain->maxFramesInFlight;

    commandBuffers.resize(frameCount);
}

void Metal_CommandBuffer::bind(LvCommandBufferUsage usage) {
    uint8_t index = std::min(g_metal_swapChain->crntFrame, uint8_t(frameCount - 1));
    commandBuffers[index] = g_metal_device->commandQueue->commandBuffer();
    g_metal_swapChain->activeCommandBuffer = this;
}

void Metal_CommandBuffer::submit(Metal_Semaphore* waitSemaphore, Metal_Semaphore* signalSemaphore) {
    uint8_t index = std::min(g_metal_swapChain->crntFrame, uint8_t(frameCount - 1));

    if (waitSemaphore != nullptr)
        dispatch_semaphore_wait(waitSemaphore->semaphore, DISPATCH_TIME_FOREVER);
    
    if (signalSemaphore != nullptr) {
        commandBuffers[index]->addCompletedHandler(^(MTL::CommandBuffer* cmd) {
            dispatch_semaphore_signal(signalSemaphore->semaphore);
        });
    }

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
