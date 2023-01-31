#include "metal/lvcore/core/uniform_buffer.hpp"

namespace lv {

Metal_UniformBuffer::Metal_UniformBuffer(size_t aSize) : size(aSize) {
    if (frameCount == 0) frameCount = g_metal_swapChain->maxFramesInFlight;

    buffers.resize(frameCount);
    for (uint8_t i = 0; i < frameCount; i++)
        buffers[i] = g_metal_device->device->newBuffer(size, MTL::ResourceStorageModeShared);
}

void Metal_UniformBuffer::upload(void* data) {
    memcpy(buffers[g_metal_swapChain->crntFrame]->contents(), data, size); 
    //buffers[g_metal_swapChain->crntFrame]->didModifyRange(NS::Range::Make(0, buffers[g_metal_swapChain->crntFrame]->length()));
}

} //namespace lv
