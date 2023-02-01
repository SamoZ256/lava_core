#ifndef LV_VULKAN_COMMAND_BUFFER_H
#define LV_VULKAN_COMMAND_BUFFER_H

#include <vector>

#include <vulkan/vulkan.h>

namespace lv {

class Vulkan_CommandBuffer {
public:
    uint8_t frameCount = 0;

    std::vector<VkCommandBuffer> commandBuffers;

    void init();

    void destroy();

    void bind();

    void unbind();

    void submit();
};

} //namespace lv

#endif
