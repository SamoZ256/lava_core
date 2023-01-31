#ifndef LV_VULKAN_ALLOCATOR_H
#define LV_VULKAN_ALLOCATOR_H

#include <vma/vk_mem_alloc.h>

namespace lv {

struct Vulkan_AllocatorCreateInfo {
    uint32_t vulkanVersion = VK_API_VERSION_1_1;
};

class Vulkan_Allocator {
public:
    VmaAllocator allocator;

    Vulkan_Allocator(Vulkan_AllocatorCreateInfo& createInfo);

    void destroy();
};

extern Vulkan_Allocator* g_vulkan_allocator;

//Implementation
/*
class Alloc {
public:
    static Allocator* g_allocator;
};
*/

} //namespace lv

#endif
