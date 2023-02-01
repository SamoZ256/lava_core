#ifndef LV_ALLOCATOR_H
#define LV_ALLOCATOR_H

#ifdef LV_BACKEND_VULKAN

#include "vulkan/lvcore/core/allocator.hpp"

#define g_allocator g_vulkan_allocator

namespace lv {

typedef Vulkan_AllocatorCreateInfo AllocatorCreateInfo;
typedef Vulkan_Allocator Allocator;

} //namespace lv

#endif

#endif
