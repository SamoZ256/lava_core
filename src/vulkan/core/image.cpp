#include "lvcore/core/image.hpp"

#include "lvcore/core/allocator.hpp"

namespace lv {

void Image::init(uint16_t width, uint16_t height) {
    if (usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT || usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)
        allocationFlags |= VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
    images.resize(frameCount);
    allocations.resize(frameCount);
    for (uint8_t i = 0; i < images.size(); i++) {
        //Creating image
        allocations[i] = ImageHelper::createImage((uint16_t)width, (uint16_t)height, format, VK_IMAGE_TILING_OPTIMAL, usage, VMA_MEMORY_USAGE_GPU_ONLY, images[i], nullptr, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, layerCount, mipCount, allocationFlags, flags);
        //ImageBuffer::transitionImageLayout(images[i], format, VK_IMAGE_LAYOUT_UNDEFINED, dstLayout, aspectMask);
    }
}

VkAttachmentDescription Image::getAttachmentDescription(/*VkImageLayout finalLayout*/) {
    VkAttachmentDescription description{};
    description.format = format;
    description.samples = VK_SAMPLE_COUNT_1_BIT;
    description.loadOp = loadOp;
    description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    description.initialLayout = crntLayout;
    description.finalLayout = finalLayout;

    return description;
}

VkAttachmentReference Image::getAttachmentReference(uint8_t index, VkImageLayout layout) {
    VkAttachmentReference reference{};
    reference.attachment = index;
    reference.layout = layout;

    return reference;
}

void Image::transitionLayout(VkImageLayout dstLayout) {
    for (uint8_t i = 0; i < images.size(); i++) {
        ImageHelper::transitionImageLayout(images[i], format, crntLayout, dstLayout, aspectMask, layerCount, mipCount);
    }
    crntLayout = dstLayout;
}

void Image::resize(uint16_t width, uint16_t height) {
    destroy();
    init(width, height);
}

void Image::destroy() {
    for (uint8_t i = 0; i < images.size(); i++) {
        vmaDestroyImage(g_allocator->allocator, images[i], allocations[i]);
    }
}

} //namespace lv
