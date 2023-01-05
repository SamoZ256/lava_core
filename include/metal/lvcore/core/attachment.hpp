#ifndef LV_ATTACHMENT_H
#define LV_ATTACHMENT_H

#include "image.hpp"

namespace lv {

struct Attachment {
    Image* image = nullptr;
    int8_t attachmentIndex = -1;
};

} //namespace lv

#endif
