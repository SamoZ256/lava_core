#ifndef LV_IMAGE_VIEW_H
#define LV_IMAGE_VIEW_H

#ifdef LV_BACKEND_VULKAN

#include "vulkan/lvcore/core/image_view.hpp"

namespace lv {

typedef Vulkan_ImageView ImageView;

} //namespace lv

#endif

#ifdef LV_BACKEND_METAL

#include "metal/lvcore/core/image_view.hpp"

namespace lv {

typedef Metal_ImageView ImageView;

} //namespace lv

#endif

#endif
