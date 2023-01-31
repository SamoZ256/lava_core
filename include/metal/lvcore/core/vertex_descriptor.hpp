#ifndef LV_METAL_VERTEX_DESCRIPTOR_H
#define LV_METAL_VERTEX_DESCRIPTOR_H

#include <Metal/Metal.hpp>

namespace lv {

class Metal_VertexDescriptor {
public:
    MTL::VertexDescriptor* vertexDesc;
    uint16_t bufferBinding = 0;

    Metal_VertexDescriptor(uint16_t aBufferBinding, size_t size);

    void addBinding(uint16_t location, MTL::VertexFormat format, uint32_t offset);
};

} //namespace lv

#endif
