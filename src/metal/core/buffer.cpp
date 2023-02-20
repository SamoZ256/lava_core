#include "metal/lvcore/core/buffer.hpp"

#include "metal/lvcore/core/device.hpp"

namespace lv {

void Metal_Buffer::init(uint8_t threadIndex, void* data, size_t aSize) {
    size = aSize;

    MTL::Buffer* stagingBuffer = g_metal_device->device->newBuffer(size, MTL::ResourceStorageModeShared);
    memcpy(stagingBuffer->contents(), data, size);
    //stagingBuffer->didModifyRange(NS::Range::Make(0, stagingBuffer->length()));

    buffer = g_metal_device->device->newBuffer(size, MTL::ResourceStorageModePrivate);

    copyBufferToBuffer(stagingBuffer, buffer, size);

    stagingBuffer->release();
}

void Metal_Buffer::destroy() {
    buffer->release();
}

void Metal_Buffer::copyBufferToBuffer(MTL::Buffer* srcBuffer, MTL::Buffer* dstBuffer, uint32_t size) {
    MTL::CommandBuffer* commandBuffer = g_metal_device->commandQueue->commandBuffer();

    // Encode a blit pass to copy data from the source buffer to the private buffer.
    MTL::BlitCommandEncoder* blitCommandEncoder = commandBuffer->blitCommandEncoder();

    blitCommandEncoder->copyFromBuffer(srcBuffer, 0, dstBuffer, 0, size);

    blitCommandEncoder->endEncoding();
    commandBuffer->commit();
}

void Metal_Buffer::copyBufferToImage(MTL::Buffer* srcBuffer, MTL::Texture* dstImage, uint32_t width, uint32_t height, uint16_t bytesPerPixel, uint8_t arrayLayer, uint8_t mipLayer) {
    MTL::CommandBuffer* commandBuffer = g_metal_device->commandQueue->commandBuffer();

    // Encode a blit pass to copy data from the source buffer to the private buffer.
    MTL::BlitCommandEncoder* blitCommandEncoder = commandBuffer->blitCommandEncoder();

    blitCommandEncoder->copyFromBuffer(srcBuffer, 0, width * bytesPerPixel, width * height * bytesPerPixel, MTL::Size(width, height, 1), dstImage, arrayLayer, mipLayer, MTL::Origin(0, 0, 0));

    blitCommandEncoder->endEncoding();
    commandBuffer->commit();
}

} //namespace lv
