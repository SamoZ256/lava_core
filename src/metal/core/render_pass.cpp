#include "metal/lvcore/core/render_pass.hpp"

#include <iostream>

namespace lv {

void Metal_RenderPass::init() {
    sortedAttachments.resize(colorAttachments.size() + (depthAttachment.index == -1 ? 0 : 1));
    for (uint8_t i = 0; i < colorAttachments.size(); i++) {
        sortedAttachments[colorAttachments[i].index] = &colorAttachments[i];
    }
    if (depthAttachment.index != -1) sortedAttachments[depthAttachment.index] = &depthAttachment;
}

} //namespace lv
