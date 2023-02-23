#include "metal/lvcore/core/buffer.hpp"

#include "metal/lvcore/core/device.hpp"
#include "metal/lvcore/core/swap_chain.hpp"

namespace lv {

void Metal_Buffer::init(size_t aSize) {
    if (frameCount == 0) frameCount = g_metal_swapChain->maxFramesInFlight;

    size = aSize;

    buffers.resize(frameCount);
    for (uint8_t i = 0; i < frameCount; i++)
        buffers[i] = g_metal_device->device->newBuffer(size, MTL::ResourceStorageModePrivate);
}

void Metal_Buffer::destroy() {
    for (uint8_t i = 0; i < frameCount; i++)
        buffers[i]->release();
}

void Metal_Buffer::copyDataTo(uint8_t threadIndex, void* data) {
    uint8_t index = std::min(g_metal_swapChain->crntFrame, uint8_t(frameCount - 1));
    if (memoryType == LV_MEMORY_TYPE_PRIVATE) {
        MTL::Buffer* stagingBuffer = g_metal_device->device->newBuffer(size, MTL::ResourceStorageModeShared);
        memcpy(stagingBuffer->contents(), data, size);

        copyBufferToBuffer(stagingBuffer, buffers[index], size);

        stagingBuffer->release();
    } else if (memoryType == LV_MEMORY_TYPE_SHARED) {
        memcpy(buffers[index]->contents(), data, size); 
    }
}

Metal_BufferInfo Metal_Buffer::descriptorInfo() {
	Metal_BufferInfo info;
	info.buffers.resize(frameCount);
	for (uint8_t i = 0; i < frameCount; i++) {
		info.buffers[i] = buffers[i];
	}
	info.descriptorType = LV_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

	return info;
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
