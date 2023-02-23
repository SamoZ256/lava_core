#include "opengl/lvcore/core/device.hpp"

namespace lv {

OpenGL_Device* g_opengl_device = nullptr;

OpenGL_Device::OpenGL_Device(OpenGL_DeviceCreateInfo& createInfo) {
    g_opengl_device = this;
}

} //namespace lv
