#include "metal/lvcore/core/render_pass.hpp"

#include <iostream>

namespace lv {

void Metal_RenderPass::init() {
    sortedAttachments.resize(attachments.size());
    for (uint8_t i = 0; i < attachments.size(); i++)
        sortedAttachments[attachments[i].index] = &attachments[i];
}

} //namespace lv
