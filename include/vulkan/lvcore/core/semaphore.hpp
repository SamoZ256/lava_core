#ifndef LV_VULKAN_SEMAPHORE_H
#define LV_VULKAN_SEMAPHORE_H

#include <vector>

#include <vulkan/vulkan.h>

namespace lv {

class Vulkan_Semaphore {
public:
    uint8_t frameCount = 0;

    std::vector<VkSemaphore> semaphores;

    void init();

    void destroy();
};

} //namespace lv

#endif
