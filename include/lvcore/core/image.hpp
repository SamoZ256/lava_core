#ifndef LV_IMAGE_H
#define LV_IMAGE_H

#ifdef LV_BACKEND_VULKAN

#include "vulkan/lvcore/core/image.hpp"

namespace lv {

typedef Vulkan_Image Image;

} //namespace lv

#endif

#ifdef LV_BACKEND_METAL

#include "metal/lvcore/core/image.hpp"

namespace lv {

typedef Metal_Image Image;

} //namespace lv

#endif

#endif
