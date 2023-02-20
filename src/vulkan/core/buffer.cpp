#include "vulkan/lvcore/core/buffer.hpp"

#include "vulkan/lvcore/core/buffer_helper.hpp"
#include "vulkan/lvcore/core/allocator.hpp"
#include "vulkan/lvcore/core/swap_chain.hpp"

namespace lv {

void Vulkan_Buffer::init(uint8_t threadIndex, void* data, uint32_t aSize) {
	size = aSize;

	allocation = createRenderBuffer(threadIndex, size, usage, buffer, data, allocationFlags);
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

VmaAllocation Vulkan_Buffer::createRenderBuffer(uint8_t threadIndex, VkDeviceSize size, VkBufferUsageFlags usage, VkBuffer &buffer, void* bufferData, VmaAllocationCreateFlags allocationFlags) {
    VkBuffer stagingBuffer;

    //VmaAllocationInfo allocInfo;
    VmaAllocation stagingAllocation = Vulkan_BufferHelper::createBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, stagingBuffer, nullptr, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT);

    void* data;
    vmaMapMemory(g_vulkan_allocator->allocator, stagingAllocation, &data);
    memcpy(data, bufferData, size);
    vmaUnmapMemory(g_vulkan_allocator->allocator, stagingAllocation);
    //memcpy(allocInfo.pMappedData, bufferData, size);

    VmaAllocation allocation = Vulkan_BufferHelper::createBuffer(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage, buffer, nullptr, 0, allocationFlags); //VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT

    Vulkan_BufferHelper::copyBuffer(threadIndex, stagingBuffer, buffer, size);

    vmaDestroyBuffer(g_vulkan_allocator->allocator, stagingBuffer, stagingAllocation);

    return allocation;
}

void Vulkan_Buffer::destroy() {
    vmaDestroyBuffer(g_vulkan_allocator->allocator, buffer, allocation);
    //vmaDestroyBuffer(g_vulkan_allocator->allocator, indexBuffer, indexAllocation);
}

void Vulkan_Buffer::bindVertexBuffer() {
    VkBuffer buffers[] = {buffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(g_vulkan_swapChain->getActiveCommandBuffer(), 0, 1, buffers, offsets);
    //vkCmdBindIndexBuffer(g_metal_swapChain->getActiveCommandBuffer(), indexBuffer, 0, VK_INDEX_TYPE_UINT32);
}

void Vulkan_Buffer::bindIndexBuffer(VkIndexType indexType) {
    vkCmdBindIndexBuffer(g_vulkan_swapChain->getActiveCommandBuffer(), buffer, 0, indexType);
}

/*
void Buffer::render() {
    vkCmdDrawIndexed(g_metal_swapChain->getActiveCommandBuffer(), indexCount, 1, 0, 0, 0);
}
*/

void Vulkan_Buffer::renderIndexed(size_t indexSize) {
    vkCmdDrawIndexed(g_vulkan_swapChain->getActiveCommandBuffer(), size / indexSize, 1, 0, 0, 0);
}

} //namespace lv
