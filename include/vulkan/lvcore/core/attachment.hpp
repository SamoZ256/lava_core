#ifndef LV_VULKAN_ATTACHMENT_H
#define LV_VULKAN_ATTACHMENT_H

#include "image_view.hpp"

namespace lv {

struct Vulkan_RenderPassAttachment {
    VkFormat format;
    int8_t index = -1;
    VkAttachmentLoadOp loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    VkAttachmentStoreOp storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    bool blendEnable = false;
    VkBlendFactor srcRgbBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    VkBlendFactor dstRgbBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    VkBlendOp rgbBlendOp = VK_BLEND_OP_ADD;
    VkBlendFactor srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    VkBlendFactor dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    VkBlendOp alphaBlendOp = VK_BLEND_OP_MAX;
    VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    VkImageLayout finalLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    VkAttachmentDescription getAttachmentDescription(/*VkImageLayout finalLayout*/) {
        VkAttachmentDescription description{};
        description.format = format;
        description.samples = VK_SAMPLE_COUNT_1_BIT;
        description.loadOp = loadOp;
        description.storeOp = storeOp;
        description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        description.initialLayout = initialLayout;
        description.finalLayout = finalLayout;

        return description;
    }

    //ImageGroup() {}

    //ImageGroup(Image* aImage, ImageView* aImageView, int8_t aAttachmentIndex) : image(aImage), imageView(aImageView), attachmentIndex(aAttachmentIndex) {}
};

struct Vulkan_SubpassAttachment {
    int8_t index = -1;
    VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;

    VkAttachmentReference getAttachmentReference() {
        VkAttachmentReference reference{};
        reference.attachment = index;
        reference.layout = layout;

        return reference;
    }
};

struct Vulkan_FramebufferAttachment {
    Vulkan_ImageView* imageView = nullptr;
    int8_t index = -1;
};

} //namespace lv

#endif
