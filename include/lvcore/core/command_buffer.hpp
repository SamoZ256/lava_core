#ifndef LV_COMMAND_BUFFER_H
#define LV_COMMAND_BUFFER_H

#ifdef LV_BACKEND_VULKAN

#include "vulkan/lvcore/core/command_buffer.hpp"

namespace lv {

typedef Vulkan_CommandBuffer CommandBuffer;

} //namespace lv

#endif

#ifdef LV_BACKEND_METAL

#include "metal/lvcore/core/command_buffer.hpp"

namespace lv {

typedef Metal_CommandBuffer CommandBuffer;

} //namespace lv

#endif

#endif
