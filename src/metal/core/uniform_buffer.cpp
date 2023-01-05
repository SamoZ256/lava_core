#include "lvcore/core/uniform_buffer.hpp"

namespace lv {

UniformBuffer::UniformBuffer(size_t aSize) : size(aSize) {
    buffers.resize(g_swapChain->maxFramesInFlight);
    for (uint8_t i = 0; i < g_swapChain->maxFramesInFlight; i++)
        buffers[i] = g_device->device->newBuffer(size, MTL::ResourceStorageModeManaged);
}

void UniformBuffer::upload(void* data) {
    memcpy(buffers[g_swapChain->crntFrame]->contents(), data, size); 
    buffers[g_swapChain->crntFrame]->didModifyRange(NS::Range::Make(0, buffers[g_swapChain->crntFrame]->length()));
}

} //namespace lv
