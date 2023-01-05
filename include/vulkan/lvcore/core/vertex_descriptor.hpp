#ifndef LV_VERTEX_DESCRIPTOR_H
#define LV_VERTEX_DESCRIPTOR_H

#include <vector>

#include <vulkan/vulkan.h>

namespace lv {

struct VertexDescription {
    std::vector<VkVertexInputBindingDescription> bindingDescriptions;
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
};

class VertexDescriptor {
public:
    VertexDescription vertexDesc;
    uint16_t bufferBinding = 0;

    VertexDescriptor(uint16_t aBufferBinding, size_t size);

    void addBinding(uint16_t location, VkFormat format, uint32_t offset);
};

} //namespace lv

#endif
