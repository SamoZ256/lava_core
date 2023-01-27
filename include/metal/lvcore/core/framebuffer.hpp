#ifndef LV_FRAMEBUFFER_H
#define LV_FRAMEBUFFER_H

#include <vector>

#include "attachment.hpp"
#include "command_buffer.hpp"

namespace lv {

class Framebuffer {
public:
    uint8_t frameCount = 0;

    std::vector<MTL::RenderPassDescriptor*> renderPasses;
    CommandBuffer commandBuffer;
    MTL::RenderCommandEncoder* encoder = nullptr;

    std::vector<Attachment> colorAttachments;
    Attachment depthAttachment;

    void init();

    void destroy();

    void addColorAttachment(Image* image, uint8_t attachmentIndex, MTL::LoadAction loadOp = MTL::LoadActionDontCare, MTL::StoreAction storeOp = MTL::StoreActionStore) {
        colorAttachments.push_back({image, attachmentIndex, loadOp, storeOp});
    }

    void setDepthAttachment(Image* image, uint8_t attachmentIndex, MTL::LoadAction loadOp = MTL::LoadActionDontCare, MTL::StoreAction storeOp = MTL::StoreActionStore) {
        depthAttachment = {image, attachmentIndex, loadOp, storeOp};
    }

    void bind();

    void unbind();

    void render();
};

} //namespace lv

#endif
