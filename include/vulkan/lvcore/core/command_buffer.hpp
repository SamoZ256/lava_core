#ifndef LV_VULKAN_COMMAND_BUFFER_H
#define LV_VULKAN_COMMAND_BUFFER_H

#include <vector>

#include "semaphore.hpp"

namespace lv {

class Vulkan_CommandBuffer {
public:
    uint8_t frameCount = 0;

    uint8_t threadIndex = 0;

    std::vector<VkCommandBuffer> commandBuffers;

    void init();

    void destroy();

    void bind(VkCommandBufferUsageFlags usage = 0);

    void unbind();

    void submit(Vulkan_Semaphore* waitSemaphore = nullptr, Vulkan_Semaphore* signalSemaphore = nullptr);
};

} //namespace lv

#endif
