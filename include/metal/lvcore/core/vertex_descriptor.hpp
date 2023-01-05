#ifndef LV_VERTEX_DESCRIPTOR_H
#define LV_VERTEX_DESCRIPTOR_H

#include <Metal/Metal.hpp>

namespace lv {

class VertexDescriptor {
public:
    MTL::VertexDescriptor* vertexDesc;
    uint16_t bufferBinding = 0;

    VertexDescriptor(uint16_t aBufferBinding, size_t size);

    void addBinding(uint16_t location, MTL::VertexFormat format, uint32_t offset);
};

} //namespace lv

#endif
