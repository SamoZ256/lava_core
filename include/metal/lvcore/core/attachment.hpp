#ifndef LV_ATTACHMENT_H
#define LV_ATTACHMENT_H

#include "image.hpp"

namespace lv {

struct Attachment {
    Image* image = nullptr;
    uint8_t attachmentIndex;
    MTL::LoadAction loadOp;
    MTL::StoreAction storeOp;
};

} //namespace lv

#endif
