#ifndef LV_METAL_COMMAND_BUFFER_H
#define LV_METAL_COMMAND_BUFFER_H

#include <vector>

#include <Metal/Metal.hpp>

#include "enums.hpp"

#include "semaphore.hpp"

namespace lv {

class Metal_CommandBuffer {
public:
    uint8_t frameCount = 0;

    uint8_t threadIndex = 0;

    std::vector<MTL::CommandBuffer*> commandBuffers;

    void init();

    void destroy() {}

    void bind(LvCommandBufferUsage usage = 0);

    void unbind() {}

    void submit(Metal_Semaphore* waitSemaphore = nullptr, Metal_Semaphore* signalSemaphore = nullptr);

    MTL::RenderCommandEncoder* createRenderCommandEncoder(MTL::RenderPassDescriptor* renderPass);

    MTL::ComputeCommandEncoder* createComputeCommandEncoder();
};

} //namespace lv

#endif
