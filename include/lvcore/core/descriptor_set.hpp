#ifndef LV_DESCRIPTOR_SET_H
#define LV_DESCRIPTOR_SET_H

#ifdef LV_BACKEND_VULKAN

#include "vulkan/lvcore/core/descriptor_set.hpp"

#define g_descriptorPool g_vulkan_descriptorPool

namespace lv {

typedef Vulkan_DescriptorSet DescriptorSet;
typedef Vulkan_DescriptorSetLayout DescriptorSetLayout;
typedef Vulkan_PipelineLayout PipelineLayout;
typedef Vulkan_DescriptorPoolCreateInfo DescriptorPoolCreateInfo;
typedef Vulkan_DescriptorPool DescriptorPool;

} //namespace lv

#endif

#endif
