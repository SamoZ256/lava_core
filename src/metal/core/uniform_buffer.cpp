#include "lvcore/core/uniform_buffer.hpp"

namespace lv {

UniformBuffer::UniformBuffer(size_t aSize) : size(aSize) {
    if (frameCount == 0) frameCount = g_swapChain->maxFramesInFlight;

    buffers.resize(frameCount);
    for (uint8_t i = 0; i < frameCount; i++)
        buffers[i] = g_device->device->newBuffer(size, MTL::ResourceStorageModeShared);
}

void UniformBuffer::upload(void* data) {
    memcpy(buffers[g_swapChain->crntFrame]->contents(), data, size); 
    //buffers[g_swapChain->crntFrame]->didModifyRange(NS::Range::Make(0, buffers[g_swapChain->crntFrame]->length()));
}

} //namespace lv
