#ifndef LV_VULKAN_VERTEX_DESCRIPTOR_H
#define LV_VULKAN_VERTEX_DESCRIPTOR_H

#include <vector>

#include <vulkan/vulkan.h>

namespace lv {

struct Vulkan_VertexDescription {
    std::vector<VkVertexInputBindingDescription> bindingDescriptions;
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
};

class Vulkan_VertexDescriptor {
public:
    Vulkan_VertexDescription vertexDesc;
    uint16_t bufferBinding = 0;

    Vulkan_VertexDescriptor(uint16_t aBufferBinding, size_t size);

    void addBinding(uint16_t location, VkFormat format, uint32_t offset);
};

} //namespace lv

#endif
