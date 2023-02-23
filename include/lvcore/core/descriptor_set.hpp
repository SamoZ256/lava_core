#ifndef LV_DESCRIPTOR_SET_H
#define LV_DESCRIPTOR_SET_H

#ifdef LV_BACKEND_VULKAN

#include "vulkan/lvcore/core/descriptor_set.hpp"

#define g_descriptorPool g_vulkan_descriptorPool

namespace lv {

typedef Vulkan_DescriptorSet DescriptorSet;
typedef Vulkan_DescriptorPoolCreateInfo DescriptorPoolCreateInfo;
typedef Vulkan_DescriptorPool DescriptorPool;

} //namespace lv

#endif

#ifdef LV_BACKEND_METAL

#include "metal/lvcore/core/descriptor_set.hpp"

namespace lv {

typedef Metal_DescriptorSet DescriptorSet;
typedef Metal_DescriptorPoolCreateInfo DescriptorPoolCreateInfo;
typedef Metal_DescriptorPool DescriptorPool;

} //namespace lv

#endif

#ifdef LV_BACKEND_OPENGL

#include "opengl/lvcore/core/descriptor_set.hpp"

namespace lv {

typedef OpenGL_DescriptorSet DescriptorSet;
typedef OpenGL_DescriptorPoolCreateInfo DescriptorPoolCreateInfo;
typedef OpenGL_DescriptorPool DescriptorPool;

} //namespace lv

#endif

#endif
