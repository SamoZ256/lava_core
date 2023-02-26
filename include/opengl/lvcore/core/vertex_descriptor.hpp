#ifndef LV_OPENGL_VERTEX_DESCRIPTOR_H
#define LV_OPENGL_VERTEX_DESCRIPTOR_H

#include <stdlib.h>
#include <vector>

#include "enums.hpp"

namespace lv {

struct OpenGL_VertexAttributeBinding {
    uint16_t location;
    uint8_t numberComponents;
    GLenum type;
    void* offset;
};

class OpenGL_VertexDescriptor {
public:
    size_t size;

    std::vector<OpenGL_VertexAttributeBinding> bindings;

    OpenGL_VertexDescriptor(size_t aSize);

    void addBinding(uint16_t location, LvVertexFormat format, uint32_t offset);
};

} //namespace lv

#endif
