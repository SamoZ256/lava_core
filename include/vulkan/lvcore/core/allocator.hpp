#ifndef LV_ALLOCATOR_H
#define LV_ALLOCATOR_H

#include <vma/vk_mem_alloc.h>

namespace lv {

struct AllocatorCreateInfo {
    uint32_t vulkanVersion = VK_API_VERSION_1_1;
};

class Allocator {
public:
    VmaAllocator allocator;

    Allocator(AllocatorCreateInfo& createInfo);

    void destroy();
};

extern Allocator* g_allocator;

//Implementation
/*
class Alloc {
public:
    static Allocator* g_allocator;
};
*/

} //namespace lv

#endif
