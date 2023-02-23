#include "vulkan/lvcore/core/vertex_descriptor.hpp"

namespace lv {

Vulkan_VertexDescriptor::Vulkan_VertexDescriptor(size_t size) {
    vertexDesc.bindingDescriptions.resize(1);
    vertexDesc.bindingDescriptions[0].binding = 0;
    vertexDesc.bindingDescriptions[0].stride = size;
    vertexDesc.bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
}

void Vulkan_VertexDescriptor::addBinding(uint16_t location, VkFormat format, uint32_t offset) {
    VkVertexInputAttributeDescription desc;
    desc.binding = 0;
    desc.location = location;
    desc.format = format;
    desc.offset = offset;

    vertexDesc.attributeDescriptions.push_back(desc);
}

} //namespace lv
