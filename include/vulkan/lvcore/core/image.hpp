#ifndef LV_IMAGE_H
#define LV_IMAGE_H

#include <cassert>
#include <stdexcept>
#include <string>
#include <vector>

#include "common.hpp"

#include "buffer_helper.hpp"
#include "image_helper.hpp"

namespace lv {

class Image {
public:
    uint8_t frameCount = MAX_FRAMES_IN_FLIGHT;

    std::vector<VkImage> images;
    std::vector<VmaAllocation> allocations;
    //std::vector<VkDeviceMemory> imageMemories;

    VkFormat format;
    VkImageUsageFlags usage = 0;// = /*VK_IMAGE_USAGE_TRANSFER_DST_BIT | */VK_IMAGE_USAGE_SAMPLED_BIT;
    VkImageAspectFlags aspectMask = 0;
    uint8_t layerCount = 1;
    uint8_t mipCount = 1;
    //VkImageLayout dstLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    VkImageCreateFlags flags = 0;
    VkImageLayout crntLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    VkImageLayout finalLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    VkAttachmentLoadOp loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	VmaAllocationCreateFlags allocationFlags = 0;

    bool resized = false;

    void init(uint16_t width, uint16_t height);

    VkAttachmentDescription getAttachmentDescription(/*VkImageLayout finalLayout*/);

    VkAttachmentReference getAttachmentReference(uint8_t index, VkImageLayout layout);

    void transitionLayout(VkImageLayout dstLayout);

    void resize(uint16_t width, uint16_t height);

    void destroy();
};

} //namespace lv

#endif
