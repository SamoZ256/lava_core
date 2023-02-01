#ifndef LV_INSTANCE_H
#define LV_INSTANCE_H

#ifdef LV_BACKEND_VULKAN

#include "vulkan/lvcore/core/instance.hpp"

#define g_instance g_vulkan_instance

namespace lv {

typedef Vulkan_InstanceCreateInfo InstanceCreateInfo;
typedef Vulkan_Instance Instance;

} //namespace lv

#endif

#endif
