#ifndef LV_UNIFORM_BUFFER_H
#define LV_UNIFORM_BUFFER_H

#ifdef LV_BACKEND_VULKAN

#include "vulkan/lvcore/core/uniform_buffer.hpp"

namespace lv {

typedef Vulkan_UniformBuffer UniformBuffer;

} //namespace lv

#endif

#ifdef LV_BACKEND_METAL

#include "metal/lvcore/core/uniform_buffer.hpp"

namespace lv {

typedef Metal_UniformBuffer UniformBuffer;

} //namespace lv

#endif

#endif
