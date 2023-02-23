#include "metal/lvcore/core/vertex_descriptor.hpp"

#include "metal/lvcore/core/common.hpp"

namespace lv {

Metal_VertexDescriptor::Metal_VertexDescriptor(size_t size) {
    vertexDesc = MTL::VertexDescriptor::alloc()->init();
    vertexDesc->layouts()->object(LV_METAL_VERTEX_BUFFER_BINDING_INDEX)->setStride(size);
}

void Metal_VertexDescriptor::addBinding(uint16_t location, MTL::VertexFormat format, uint32_t offset) {
    vertexDesc->attributes()->object(location)->setFormat(format);
    vertexDesc->attributes()->object(location)->setBufferIndex(LV_METAL_VERTEX_BUFFER_BINDING_INDEX);
    vertexDesc->attributes()->object(location)->setOffset(offset);
}

} //namespace lv
