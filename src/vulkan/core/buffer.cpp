#include "lvcore/core/buffer.hpp"

#include "lvcore/core/buffer_helper.hpp"
#include "lvcore/core/allocator.hpp"
#include "lvcore/core/swap_chain.hpp"

namespace lv {

void Buffer::init(void* data, uint32_t aSize) {
	size = aSize;

	allocation = createRenderBuffer(size, usage, buffer, data, allocationFlags);
	/*
    VkDeviceSize vertexBufferSize = verticesSize;

    indexCount = static_cast<uint32_t>(indices.size());
    VkDeviceSize indexBufferSize = sizeof(uint32_t) * indexCount;

    vertexAllocation = createRenderBuffer(vertexBufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, vertexBuffer, verticesData);
    indexAllocation = createRenderBuffer(indexBufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, indexBuffer, indices.data());
	*/
    /*
    for (auto& vertex : vertices) {
      	std::cout << "Position: " << vertex.position.x << ", " << vertex.position.y << ", " << vertex.position.z << std::endl;
    }
    for (auto& index : indices) {
      	std::cout << "Index: " << index << std::endl;
    }
    */
}

VmaAllocation Buffer::createRenderBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkBuffer &buffer, void* bufferData, VmaAllocationCreateFlags allocationFlags) {
    VkBuffer stagingBuffer;

    //VmaAllocationInfo allocInfo;
    VmaAllocation stagingAllocation = BufferHelper::createBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY, stagingBuffer, nullptr, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT);

    void* data;
    vmaMapMemory(g_allocator->allocator, stagingAllocation, &data);
    memcpy(data, bufferData, size);
    vmaUnmapMemory(g_allocator->allocator, stagingAllocation);
    //memcpy(allocInfo.pMappedData, bufferData, size);

    VmaAllocation allocation = BufferHelper::createBuffer(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage, VMA_MEMORY_USAGE_GPU_ONLY, buffer, nullptr, 0, allocationFlags); //VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT

    BufferHelper::copyBuffer(stagingBuffer, buffer, size);

    vmaDestroyBuffer(g_allocator->allocator, stagingBuffer, stagingAllocation);

    return allocation;
}

void Buffer::destroy() {
    vmaDestroyBuffer(g_allocator->allocator, buffer, allocation);
    //vmaDestroyBuffer(g_allocator->allocator, indexBuffer, indexAllocation);
}

void Buffer::bindVertexBuffer() {
    VkBuffer buffers[] = {buffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(g_swapChain->getActiveCommandBuffer(), 0, 1, buffers, offsets);
    //vkCmdBindIndexBuffer(g_swapChain->getActiveCommandBuffer(), indexBuffer, 0, VK_INDEX_TYPE_UINT32);
}

void Buffer::bindIndexBuffer(VkIndexType indexType) {
    vkCmdBindIndexBuffer(g_swapChain->getActiveCommandBuffer(), buffer, 0, indexType);
}

/*
void Buffer::render() {
    vkCmdDrawIndexed(g_swapChain->getActiveCommandBuffer(), indexCount, 1, 0, 0, 0);
}
*/

void Buffer::renderIndexed(size_t indexSize) {
    vkCmdDrawIndexed(g_swapChain->getActiveCommandBuffer(), size / indexSize, 1, 0, 0, 0);
}

} //namespace lv
