#ifndef LV_OPENGL_INSTANCE_H
#define LV_OPENGL_INSTANCE_H

#include <stdint.h>

#define LV_MAKE_VERSION(variant, major, minor, patch) 0

#define LV_VULKAN_VERSION_1_0 0
#define LV_VULKAN_VERSION_1_1 0
#define LV_VULKAN_VERSION_1_2 0
#define LV_VULKAN_VERSION_1_3 0

namespace lv {

struct OpenGL_InstanceCreateInfo {
    const char* applicationName;
    uint32_t applicationVersion = LV_MAKE_VERSION(0, 1, 0, 0);
    uint32_t vulkanVersion = LV_VULKAN_VERSION_1_0;
    bool validationEnable = false;
};

class OpenGL_Instance {
public:
    bool validationEnable;

    OpenGL_Instance(OpenGL_InstanceCreateInfo& createInfo);

    void destroy() {}
};

extern OpenGL_Instance* g_opengl_instance;

} //namespace lv

#endif
