#ifndef LV_FRAMEBUFFER_H
#define LV_FRAMEBUFFER_H

#ifdef LV_BACKEND_VULKAN

#include "vulkan/lvcore/core/framebuffer.hpp"

namespace lv {

typedef Vulkan_Framebuffer Framebuffer;

} //namespace lv

#endif

#ifdef LV_BACKEND_METAL

#include "metal/lvcore/core/framebuffer.hpp"

namespace lv {

typedef Metal_Framebuffer Framebuffer;

} //namespace lv

#endif

#endif
