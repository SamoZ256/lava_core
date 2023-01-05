#ifndef LV_BUFFER_H
#define LV_BUFFER_H

#include "swap_chain.hpp"

namespace lv {

class Buffer {
public:
    MTL::Buffer* buffer;
    size_t size;

    void init(void* data, size_t aSize);

    void destroy();

    void bindVertexBuffer(uint16_t index) { g_swapChain->activeFramebuffer->encoder->setVertexBuffer(buffer, 0, index); }

    void render(size_t vertexSize) { g_swapChain->activeFramebuffer->encoder->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(size / vertexSize)); }

    void renderIndexed(MTL::IndexType indexType, size_t indexSize) { g_swapChain->activeFramebuffer->encoder->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, size / indexSize, indexType, buffer, 0); }
};

} //namespace lv

#endif
