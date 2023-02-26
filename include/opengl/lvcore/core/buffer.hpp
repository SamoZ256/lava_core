#ifndef LV_OPENGL_BUFFER_H
#define LV_OPENGL_BUFFER_H

#include <stdlib.h>

#include "enums.hpp"

namespace lv {

struct OpenGL_BufferInfo {
    GLuint buffer;
    LvDescriptorType descriptorType;
};

//TODO: check if the buffer is allocated whenever using it
class OpenGL_Buffer {
public:
    uint8_t frameCount = 0;

    GLuint buffer;

    GLuint vertexAttributeObject;
    bool hasVertexArrayObject = false;
    bool createdVertexArrayObject = false;

    size_t size;

    LvBufferUsageFlags usage = 0;
    LvMemoryType memoryType = LV_MEMORY_TYPE_PRIVATE;
    LvMemoryAllocationCreateFlags memoryAllocationFlags = 0;

    void init(size_t aSize);

    void destroy();

    void copyDataTo(uint8_t threadIndex, void* data);

    void bindVertexBuffer();

    void bindIndexBuffer(LvIndexType aIndexType);

    void render(size_t vertexSize);

    void renderIndexed(size_t indexSize);

    OpenGL_BufferInfo descriptorInfo();

private:
    LvIndexType indexType;
    GLenum bindTarget = 0;
    GLenum bufferUsage = GL_DYNAMIC_DRAW;
    bool allocated;
};

} //namespace lv

#endif
