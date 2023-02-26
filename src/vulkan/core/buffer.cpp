#include "vulkan/lvcore/core/buffer.hpp"

#include "vulkan/lvcore/core/buffer_helper.hpp"
#include "vulkan/lvcore/core/swap_chain.hpp"

namespace lv {

void Vulkan_Buffer::init(size_t aSize) {
    if (frameCount == 0) frameCount = g_vulkan_swapChain->maxFramesInFlight;

	size = aSize;

    buffers.resize(frameCount);
    allocations.resize(frameCount);
    for (uint8_t i = 0; i < frameCount; i++)
        allocations[i] = Vulkan_BufferHelper::createBuffer(size, usage, buffers[i], nullptr, memoryType, memoryAllocationFlags); //VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
}

void Vulkan_Buffer::destroy() {
    for (uint8_t i = 0; i < frameCount; i++)
        vmaDestroyBuffer(g_vulkan_device->allocator, buffers[i], allocations[i]);
}

void Vulkan_Buffer::copyDataTo(uint8_t threadIndex, void* data) {
    uint8_t index = std::min(g_vulkan_swapChain->crntFrame, uint8_t(frameCount - 1));
    if (memoryType == LV_MEMORY_TYPE_PRIVATE) {
        VkBuffer stagingBuffer;

        VmaAllocation stagingAllocation = Vulkan_BufferHelper::createBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, stagingBuffer, nullptr, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT);

        void* mappedData;
        vmaMapMemory(g_vulkan_device->allocator, stagingAllocation, &mappedData);
        memcpy(mappedData, data, size);
        vmaUnmapMemory(g_vulkan_device->allocator, stagingAllocation);

        Vulkan_BufferHelper::copyBuffer(threadIndex, stagingBuffer, buffers[index], size);

        vmaDestroyBuffer(g_vulkan_device->allocator, stagingBuffer, stagingAllocation);
    } else if (memoryType == LV_MEMORY_TYPE_SHARED) {
        void* mappedData;
        vmaMapMemory(g_vulkan_device->allocator, allocations[g_vulkan_swapChain->imageIndex], &mappedData);
        memcpy(mappedData, data, size);
        vmaUnmapMemory(g_vulkan_device->allocator, allocations[g_vulkan_swapChain->imageIndex]);
    }
}

void Vulkan_Buffer::bindVertexBuffer() {
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(g_vulkan_swapChain->getActiveCommandBuffer(), 0, 1, &buffers[std::min(g_vulkan_swapChain->crntFrame, uint8_t(frameCount - 1))], offsets);
}

void Vulkan_Buffer::bindIndexBuffer(VkIndexType indexType) {
    vkCmdBindIndexBuffer(g_vulkan_swapChain->getActiveCommandBuffer(), buffers[std::min(g_vulkan_swapChain->crntFrame, uint8_t(frameCount - 1))], 0, indexType);
}

void Vulkan_Buffer::render(size_t vertexSize) {
    vkCmdDraw(g_vulkan_swapChain->getActiveCommandBuffer(), size / vertexSize, 1, 0, 0);
}

void Vulkan_Buffer::renderIndexed(size_t indexSize) {
    vkCmdDrawIndexed(g_vulkan_swapChain->getActiveCommandBuffer(), size / indexSize, 1, 0, 0, 0);
}

Vulkan_BufferInfo Vulkan_Buffer::descriptorInfo(/*VkDeviceSize size, VkDeviceSize offset*/) {
	Vulkan_BufferInfo info;
	info.infos.resize(frameCount);
	for (uint8_t i = 0; i < frameCount; i++) {
		info.infos[i].buffer = buffers[i];
		info.infos[i].offset = 0;//offset;
		info.infos[i].range = VK_WHOLE_SIZE;//size;
	}
	info.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

	return info;
}

} //namespace lv
