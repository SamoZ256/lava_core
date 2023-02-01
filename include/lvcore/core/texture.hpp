#ifndef LV_TEXTURE_H
#define LV_TEXTURE_H

#ifdef LV_BACKEND_VULKAN

#include "vulkan/lvcore/core/texture.hpp"

namespace lv {

typedef Vulkan_Texture Texture;

} //namespace lv

#endif

#ifdef LV_BACKEND_METAL

#include "metal/lvcore/core/texture.hpp"

namespace lv {

typedef Metal_Texture Texture;

} //namespace lv

#endif

#endif
