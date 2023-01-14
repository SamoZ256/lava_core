#ifndef LV_BUFFER_H
#define LV_BUFFER_H

#include <vector>

#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>

namespace lv {

class Buffer {
public:
	/*
	VkBuffer vertexBuffer;
	VmaAllocation vertexAllocation;
	VkBuffer indexBuffer;
	VmaAllocation indexAllocation;

	uint32_t indexCount;
	*/
	VkBuffer buffer;
	VmaAllocation allocation;
	VmaAllocationCreateFlags allocationFlags = 0;

	size_t size;

	VkBufferUsageFlags usage = 0;
	
	void init(void* data, uint32_t aSize);

	void destroy();

	static VmaAllocation createRenderBuffer(
		VkDeviceSize size,
		VkBufferUsageFlags usage,
		VkBuffer &buffer,
		void* bufferData,
		VmaAllocationCreateFlags allocationFlags
	);

	void bindVertexBuffer();

	void bindIndexBuffer(VkIndexType indexType);

	void render();

	void renderIndexed(size_t indexSize);
};

} //namespace lv

#endif
