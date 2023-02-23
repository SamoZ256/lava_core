#ifndef LV_OPENGL_COMMAND_BUFFER_H
#define LV_OPENGL_COMMAND_BUFFER_H

#include "enums.hpp"

namespace lv {

class OpenGL_CommandBuffer {
public:
    uint8_t frameCount = 0;

    uint8_t threadIndex = 0;

    void init() {}

    void destroy() {}

    void bind(LvCommandBufferUsageFlags usage = 0) {}

    void unbind() {}

    //TODO: uncomment
    //void submit(OpenGL_Semaphore* waitSemaphore = nullptr, OpenGL_Semaphore* signalSemaphore = nullptr) {}
};

} //namespace lv

#endif
