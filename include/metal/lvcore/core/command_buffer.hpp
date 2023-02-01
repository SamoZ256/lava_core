#ifndef LV_METAL_COMMAND_BUFFER_H
#define LV_METAL_COMMAND_BUFFER_H

#include <vector>

#include <Metal/Metal.hpp>

namespace lv {

class Metal_CommandBuffer {
public:
    uint8_t frameCount = 0;

    std::vector<MTL::CommandBuffer*> commandBuffers;

    void init();

    void bind();

    void submit();

    MTL::RenderCommandEncoder* createRenderCommandEncoder(MTL::RenderPassDescriptor* renderPass);

    MTL::ComputeCommandEncoder* createComputeCommandEncoder();
};

} //namespace lv

#endif
