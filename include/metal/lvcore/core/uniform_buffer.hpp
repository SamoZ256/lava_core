#ifndef LV_UNIFORM_BUFFER_H
#define LV_UNIFORM_BUFFER_H

#include "swap_chain.hpp"
#include "device.hpp"

namespace lv {

class UniformBuffer {
public:
    uint8_t frameCount = 0;

    std::vector<MTL::Buffer*> buffers;
    size_t size;

    UniformBuffer(size_t aSize);

    void upload(void* data);

    void bindToVertexShader(uint16_t index) { g_swapChain->activeRenderEncoder->setVertexBuffer(buffers[g_swapChain->crntFrame], 0, index); }

    void bindToFragmentShader(uint16_t index) { g_swapChain->activeRenderEncoder->setFragmentBuffer(buffers[g_swapChain->crntFrame], 0, index); }

    void destroy() { for (uint8_t i = 0; i < buffers.size(); i++) { buffers[i]->release(); } }
};

} //namespace lv

#endif
