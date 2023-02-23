#ifndef LV_OPENGL_DEVICE_H
#define LV_OPENGL_DEVICE_H

#include "lvnd/lvnd.h"

#include "lvcore/threading/thread_pool.hpp"

namespace lv {

struct OpenGL_DeviceCreateInfo {
    LvndWindow* window;
    ThreadPool* threadPool;
};

class OpenGL_Device {
public:
    OpenGL_Device(OpenGL_DeviceCreateInfo& createInfo);

    void destroy() {}

    void waitIdle() {}
};

extern OpenGL_Device* g_opengl_device;

} //namespace lv

#endif
