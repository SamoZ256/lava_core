#include "lvcore/core/buffer.hpp"

#include "lvcore/core/device.hpp"

namespace lv {

void Buffer::init(void* data, size_t aSize) {
    size = aSize;

    MTL::Buffer* stagingBuffer = g_device->device->newBuffer(size, MTL::ResourceStorageModeShared);
    memcpy(stagingBuffer->contents(), data, size);
    //stagingBuffer->didModifyRange(NS::Range::Make(0, stagingBuffer->length()));

    buffer = g_device->device->newBuffer(size, MTL::ResourceStorageModePrivate);

    copyBufferToBuffer(stagingBuffer, buffer, size);

    stagingBuffer->release();
}

void Buffer::destroy() {
    buffer->release();
}

void Buffer::copyBufferToBuffer(MTL::Buffer* srcBuffer, MTL::Buffer* dstBuffer, uint32_t size) {
    MTL::CommandBuffer* commandBuffer = g_device->commandQueue->commandBuffer();

    // Encode a blit pass to copy data from the source buffer to the private buffer.
    MTL::BlitCommandEncoder* blitCommandEncoder = commandBuffer->blitCommandEncoder();

    blitCommandEncoder->copyFromBuffer(srcBuffer, 0, dstBuffer, 0, size);

    blitCommandEncoder->endEncoding();
    commandBuffer->commit();
}

void Buffer::copyBufferToImage(MTL::Buffer* srcBuffer, MTL::Texture* dstImage, uint32_t width, uint32_t height, uint16_t bytesPerPixel, uint8_t arrayLayer, uint8_t mipLayer) {
    MTL::CommandBuffer* commandBuffer = g_device->commandQueue->commandBuffer();

    // Encode a blit pass to copy data from the source buffer to the private buffer.
    MTL::BlitCommandEncoder* blitCommandEncoder = commandBuffer->blitCommandEncoder();

    blitCommandEncoder->copyFromBuffer(srcBuffer, 0, width * bytesPerPixel, width * height * bytesPerPixel, MTL::Size(width, height, 1), dstImage, arrayLayer, mipLayer, MTL::Origin(0, 0, 0));

    blitCommandEncoder->endEncoding();
    commandBuffer->commit();
}

} //namespace lv
