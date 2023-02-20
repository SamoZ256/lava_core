#ifndef LV_VULKAN_SEMAPHORE_H
#define LV_VULKAN_SEMAPHORE_H

#include <vector>

#include <vulkan/vulkan.h>

namespace lv {

class Vulkan_Semaphore {
public:
    int8_t frameCount = -1;

    std::vector<VkSemaphore> semaphores;

    void init();

    void destroy();
};

} //namespace lv

#endif
