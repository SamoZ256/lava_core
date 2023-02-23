#ifndef LV_METAL_BUFFER_H
#define LV_METAL_BUFFER_H

#include "common.hpp"

#include "enums.hpp"

#include "swap_chain.hpp"

namespace lv {

struct Metal_BufferInfo {
    std::vector<MTL::Buffer*> buffers;
    LvDescriptorType descriptorType;
};

class Metal_Buffer {
public:
    uint8_t frameCount = 0;

    std::vector<MTL::Buffer*> buffers;
    size_t size;

    LvBufferUsageFlags usage = 0;
    LvMemoryType memoryType = LV_MEMORY_TYPE_PRIVATE;
    LvMemoryAllocationCreateFlags memoryAllocationFlags = 0;

    void init(size_t aSize);

    void destroy();

    void copyDataTo(uint8_t threadIndex, void* data);

    void bindVertexBuffer() { g_metal_swapChain->activeRenderEncoder->setVertexBuffer(buffers[std::min(g_metal_swapChain->crntFrame, uint8_t(frameCount - 1))], 0, LV_METAL_VERTEX_BUFFER_BINDING_INDEX); }

    void bindIndexBuffer(MTL::IndexType aIndexType) { indexType = aIndexType; }

    void render(size_t vertexSize) { g_metal_swapChain->activeRenderEncoder->drawPrimitives(MTL::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(size / vertexSize)); }

    void renderIndexed(size_t indexSize) { g_metal_swapChain->activeRenderEncoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle, size / indexSize, indexType, buffers[std::min(g_metal_swapChain->crntFrame, uint8_t(frameCount - 1))], 0); }

    Metal_BufferInfo descriptorInfo();

    static void copyBufferToBuffer(MTL::Buffer* srcBuffer, MTL::Buffer* dstBuffer, uint32_t size);

    static void copyBufferToImage(MTL::Buffer* srcBuffer, MTL::Texture* dstImage, uint32_t width, uint32_t height, uint16_t bytesPerPixel = 4, uint8_t arrayLayer = 0, uint8_t mipLayer = 0);

private:
    MTL::IndexType indexType;
};

} //namespace lv

#endif
