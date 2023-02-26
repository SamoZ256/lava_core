#include "opengl/lvcore/core/vertex_descriptor.hpp"

#include <iostream>

namespace lv {

OpenGL_VertexDescriptor::OpenGL_VertexDescriptor(size_t aSize) {
    size = aSize;
}

void OpenGL_VertexDescriptor::addBinding(uint16_t location, LvVertexFormat format, uint32_t offset) {
    uint8_t numberComponents;
    GLenum type;
    LV_OPENGL_VERTEX_FORMAT_GET(format, numberComponents, type);
    bindings.push_back({location, numberComponents, type, (void*)(uintptr_t)offset});
}

} //namespace lv
