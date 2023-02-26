#ifndef LV_VULKAN_BUFFER_H
#define LV_VULKAN_BUFFER_H

#include <vector>

#include "enums.hpp"

namespace lv {

struct Vulkan_BufferInfo {
    std::vector<VkDescriptorBufferInfo> infos;
    VkDescriptorType descriptorType;
};

class Vulkan_Buffer {
public:
	uint8_t frameCount = 0;
	
	std::vector<VkBuffer> buffers;
	std::vector<VmaAllocation> allocations;

	size_t size;

	VkBufferUsageFlags usage = 0;
    VkMemoryPropertyFlags memoryType = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    VmaAllocationCreateFlags memoryAllocationFlags = 0;
	
	void init(size_t aSize);

    void destroy();

    void copyDataTo(uint8_t threadIndex, void* data);

	void bindVertexBuffer();

	void bindIndexBuffer(VkIndexType indexType);

	void render(size_t vertexSize);

	void renderIndexed(size_t indexSize);

	Vulkan_BufferInfo descriptorInfo(/*VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0*/);
};

} //namespace lv

#endif
