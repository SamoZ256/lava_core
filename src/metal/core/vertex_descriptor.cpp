#include "lvcore/core/vertex_descriptor.hpp"

namespace lv {

VertexDescriptor::VertexDescriptor(uint16_t aBufferBinding, size_t size) : bufferBinding(aBufferBinding) {
    vertexDesc = MTL::VertexDescriptor::alloc()->init();
    vertexDesc->layouts()->object(bufferBinding)->setStride(size);
}

void VertexDescriptor::addBinding(uint16_t location, MTL::VertexFormat format, uint32_t offset) {
    vertexDesc->attributes()->object(location)->setFormat(format);
    vertexDesc->attributes()->object(location)->setBufferIndex(bufferBinding);
    vertexDesc->attributes()->object(location)->setOffset(offset);
}

} //namespace lv
