#ifndef LV_VULKAN_IMAGE_H
#define LV_VULKAN_IMAGE_H

#include <cassert>
#include <stdexcept>
#include <string>
#include <vector>

#include "common.hpp"

#include "enums.hpp"

#include "buffer_helper.hpp"
#include "image_helper.hpp"

namespace lv {

class Vulkan_Image {
public:
    uint8_t frameCount = 0;

    std::vector<VkImage> images;
    std::vector<VmaAllocation> allocations;

    uint16_t width, height;

    LvFormat format;
    LvImageUsageFlags usage = 0;
    LvImageAspectFlags aspectMask = LV_IMAGE_ASPECT_COLOR_BIT;
    LvImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D;
    LvMemoryType memoryType = LV_MEMORY_TYPE_PRIVATE;
    LvMemoryAllocationCreateFlags memoryAllocationFlags = 0;
    uint16_t layerCount = 1;
    uint16_t mipCount = 1;

    bool resized = false;

    void init(uint16_t aWidth, uint16_t aHeight);

    void initFromFile(const char* filename);

    void destroy();

    void copyDataTo(uint8_t threadIndex, void* data);

    void transitionLayout(uint8_t threadIndex, uint8_t imageIndex, LvImageLayout srcLayout, LvImageLayout dstLayout);

    void resize(uint16_t width, uint16_t height);

    void generateMipmaps(uint8_t threadIndex);

    void copyToFromImage(uint8_t threadIndex, Vulkan_Image& source);

    void blitToFromImage(uint8_t threadIndex, Vulkan_Image& source);
};

} //namespace lv

#endif
