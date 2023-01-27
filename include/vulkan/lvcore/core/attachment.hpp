#ifndef LV_ATTACHMENT_H
#define LV_ATTACHMENT_H

#include "image_view.hpp"

namespace lv {

struct Attachment {
    Image* image = nullptr;
    ImageView* imageView = nullptr;
    uint8_t attachmentIndex;
    VkAttachmentLoadOp loadOp;
    VkAttachmentStoreOp storeOp;

    VkAttachmentDescription getAttachmentDescription(/*VkImageLayout finalLayout*/) {
        VkAttachmentDescription description{};
        description.format = image->format;
        description.samples = VK_SAMPLE_COUNT_1_BIT;
        description.loadOp = loadOp;
        description.storeOp = storeOp;
        description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        description.initialLayout = image->initialLayout;
        description.finalLayout = image->finalLayout;

        return description;
    }

    VkAttachmentReference getAttachmentReference(uint8_t index, VkImageLayout layout) {
        VkAttachmentReference reference{};
        reference.attachment = index;
        reference.layout = layout;

        return reference;
    }

    //ImageGroup() {}

    //ImageGroup(Image* aImage, ImageView* aImageView, int8_t aAttachmentIndex) : image(aImage), imageView(aImageView), attachmentIndex(aAttachmentIndex) {}
};

} //namespace lv

#endif
