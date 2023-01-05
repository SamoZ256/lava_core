#ifndef LV_ATTACHMENT_H
#define LV_ATTACHMENT_H

#include "image_view.hpp"

namespace lv {

struct Attachment {
    Image* image = nullptr;
    ImageView* imageView = nullptr;
    int8_t attachmentIndex = -1;

    //ImageGroup() {}

    //ImageGroup(Image* aImage, ImageView* aImageView, int8_t aAttachmentIndex) : image(aImage), imageView(aImageView), attachmentIndex(aAttachmentIndex) {}
};

} //namespace lv

#endif
