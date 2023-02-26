#include "opengl/lvcore/core/buffer.hpp"

#include <iostream>

#include "lvcore/common.hpp"

#include "opengl/lvcore/core/swap_chain.hpp"

namespace lv {

void OpenGL_Buffer::init(size_t aSize) {
    size = aSize;

    if (usage & LV_BUFFER_USAGE_VERTEX_BUFFER_BIT) {
        bindTarget = GL_ARRAY_BUFFER;

        glGenVertexArrays(1, &vertexAttributeObject);
        hasVertexArrayObject = true;
    } else if (usage & LV_BUFFER_USAGE_INDEX_BUFFER_BIT) {
        bindTarget = GL_ELEMENT_ARRAY_BUFFER;
    } else if (usage & LV_BUFFER_USAGE_UNIFORM_BUFFER_BIT) {
        bindTarget = GL_UNIFORM_BUFFER;
    }
    
    if (memoryType == LV_MEMORY_TYPE_PRIVATE)
        bufferUsage = GL_STATIC_DRAW;
    else if (usage == LV_BUFFER_USAGE_TRANSFER_SRC_BIT)
        bufferUsage = GL_STREAM_READ;

    glGenBuffers(1, &buffer);
    allocated = (bufferUsage != GL_STATIC_DRAW);
    if (allocated) {
        glBindBuffer(bindTarget, buffer);
        glBufferData(bindTarget, size, nullptr, bufferUsage);
    }
}

void OpenGL_Buffer::destroy() {
    glDeleteBuffers(1, &buffer);
}

void OpenGL_Buffer::copyDataTo(uint8_t threadIndex, void* data) {
    glBindBuffer(bindTarget, buffer);
    if (allocated) {
        glBufferSubData(bindTarget, 0, size, data);
    } else {
        glBufferData(bindTarget, size, data, bufferUsage);
        allocated = true;
    }
}

void OpenGL_Buffer::bindVertexBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    if (hasVertexArrayObject) {
        glBindVertexArray(vertexAttributeObject);
        if (!createdVertexArrayObject) {
            OpenGL_VertexDescriptor* vertexDescriptor = g_opengl_swapChain->activeGraphicsPipeline->createInfo.vertexDescriptor;
            if (!vertexDescriptor)
                LV_ERROR("Vertex buffer is bound, but no vertex descriptor specified in currently bound graphics pipeline");
            for (auto& binding : vertexDescriptor->bindings) {
                if (binding.type == GL_FLOAT) {
                    glVertexAttribPointer(binding.location, binding.numberComponents, binding.type, GL_FALSE, vertexDescriptor->size, (void*)(uintptr_t)binding.offset);
                } else
                    glVertexAttribIPointer(binding.location, binding.numberComponents, binding.type, vertexDescriptor->size, (void*)(uintptr_t)binding.offset); //TODO: add support for other formats than GL_*_BYTE by getting the size

                glEnableVertexAttribArray(binding.location);
            }

            createdVertexArrayObject = true;
        }
    }
}

void OpenGL_Buffer::bindIndexBuffer(LvIndexType aIndexType) {
    indexType = aIndexType;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
}

void OpenGL_Buffer::render(size_t vertexSize) {
    glDrawArrays(GL_TRIANGLES, 0, size / vertexSize);
}

void OpenGL_Buffer::renderIndexed(size_t indexSize) {
    glDrawElements(GL_TRIANGLES, size / indexSize, indexType, 0);
}

OpenGL_BufferInfo OpenGL_Buffer::descriptorInfo() {
    OpenGL_BufferInfo info;
    info.buffer = buffer;
    info.descriptorType = LV_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

    return info;
}

} //namespace lv
