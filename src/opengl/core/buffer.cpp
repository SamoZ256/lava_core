#include "opengl/lvcore/core/buffer.hpp"

namespace lv {

void OpenGL_Buffer::init(size_t aSize) {
    size = aSize;

    if (usage & LV_BUFFER_USAGE_VERTEX_BUFFER_BIT)
        bindTarget = GL_ARRAY_BUFFER;
    else if (usage & LV_BUFFER_USAGE_INDEX_BUFFER_BIT)
        bindTarget = GL_ELEMENT_ARRAY_BUFFER;
    else if (usage & LV_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
        bindTarget = GL_UNIFORM_BUFFER;
    
    //TODO: find the most optimal bufferUsage

    glGenBuffers(1, &buffer);
    glBindBuffer(bindTarget, buffer);

    glBufferData(bindTarget, size, nullptr, bufferUsage);
}

void OpenGL_Buffer::copyDataTo(uint8_t threadIndex, void* data) {
    glBindBuffer(bindTarget, buffer);
    glBufferSubData(bindTarget, 0, size, data);
}

void OpenGL_Buffer::bindIndexBuffer(LvIndexType aIndexType) {
    indexType = aIndexType;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
}

} //namespace lv
