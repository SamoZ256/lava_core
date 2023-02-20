#ifndef LV_METAL_BUFFER_H
#define LV_METAL_BUFFER_H

#include "swap_chain.hpp"

namespace lv {

class Metal_Buffer {
public:
    MTL::Buffer* buffer;
    size_t size;

    MTL::IndexType indexType;

    void init(uint8_t threadIndex, void* data, size_t aSize);

    void destroy();

    void bindVertexBuffer(uint16_t index) { g_metal_swapChain->activeRenderEncoder->setVertexBuffer(buffer, 0, index); }

    void bindIndexBuffer(MTL::IndexType aIndexType) { indexType = aIndexType; }

    void render(size_t vertexSize) { g_metal_swapChain->activeRenderEncoder->drawPrimitives(MTL::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(size / vertexSize)); }

    void renderIndexed(size_t indexSize) { g_metal_swapChain->activeRenderEncoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle, size / indexSize, indexType, buffer, 0); }

    static void copyBufferToBuffer(MTL::Buffer* srcBuffer, MTL::Buffer* dstBuffer, uint32_t size);

    static void copyBufferToImage(MTL::Buffer* srcBuffer, MTL::Texture* dstImage, uint32_t width, uint32_t height, uint16_t bytesPerPixel = 4, uint8_t arrayLayer = 0, uint8_t mipLayer = 0);
};

} //namespace lv

#endif
