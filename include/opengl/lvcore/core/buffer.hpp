#ifndef LV_OPENGL_BUFFER_H
#define LV_OPENGL_BUFFER_H

#include <stdlib.h>

#include "enums.hpp"

namespace lv {

class OpenGL_Buffer {
public:
    uint8_t frameCount = 0;

    GLenum buffer;
    size_t size;

    LvBufferUsageFlags usage = 0;

    void init(size_t aSize);

    void destroy() { glDeleteBuffers(1, &buffer); }

    void copyDataTo(uint8_t threadIndex, void* data);

    void bindVertexBuffer() { glBindBuffer(GL_ARRAY_BUFFER, buffer); }

    void bindIndexBuffer(LvIndexType aIndexType);

    void render(size_t vertexSize) { glDrawArrays(GL_TRIANGLES, 0, size / vertexSize); }

    void renderIndexed(size_t indexSize) { glDrawElements(GL_TRIANGLES, size / indexSize, indexType, 0); }

private:
    LvIndexType indexType;
    GLenum bindTarget = 0;
    GLenum bufferUsage = GL_DYNAMIC_DRAW;
};

} //namespace lv

#endif
