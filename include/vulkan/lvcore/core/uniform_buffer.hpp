#ifndef LV_UNIFORM_BUFFER_H
#define LV_UNIFORM_BUFFER_H

#include <vector>

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace lv {

struct BufferInfo {
  std::vector<VkDescriptorBufferInfo> infos;
};

class UniformBuffer {
public:
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
