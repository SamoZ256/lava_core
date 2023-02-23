#include "opengl/lvcore/core/instance.hpp"

namespace lv {

OpenGL_Instance* g_opengl_instance = nullptr;

OpenGL_Instance::OpenGL_Instance(OpenGL_InstanceCreateInfo& createInfo) {
    validationEnable = createInfo.validationEnable;

    g_opengl_instance = this;
}

} //namespace lv
