#ifndef LV_UNIFORM_BUFFER_H
#define LV_UNIFORM_BUFFER_H

#include <vector>

#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>

namespace lv {

struct BufferInfo {
    std::vector<VkDescriptorBufferInfo> infos;
    VkDescriptorType descriptorType;
};

class UniformBuffer {
public:
    uint8_t frameCount = 0;

    std::vector<VkBuffer> buffers;
    std::vector<VmaAllocation> allocations;
    //std::vector<VmaAllocationInfo> allocInfos;
    size_t size;

    UniformBuffer(size_t aSize);

    void destroy();

    //VkDescriptorSetLayoutBinding getBinding(uint8_t binding);

    BufferInfo descriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

    void upload(void* uploadData);
};

} //namespace lv

#endif
