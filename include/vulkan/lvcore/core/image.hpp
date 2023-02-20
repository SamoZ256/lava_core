#ifndef LV_VULKAN_IMAGE_H
#define LV_VULKAN_IMAGE_H

#include <cassert>
#include <stdexcept>
#include <string>
#include <vector>

#include "common.hpp"

#include "buffer_helper.hpp"
#include "image_helper.hpp"

namespace lv {

class Vulkan_Image {
public:
    uint8_t frameCount = 0;

    std::vector<VkImage> images;
    std::vector<VmaAllocation> allocations;
    //std::vector<VkDeviceMemory> imageMemories;

    uint16_t width, height;

    VkFormat format;
    VkImageUsageFlags usage = 0;// = /*VK_IMAGE_USAGE_TRANSFER_DST_BIT | */VK_IMAGE_USAGE_SAMPLED_BIT;
    VkImageAspectFlags aspectMask = 0;
    VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D;
    VkMemoryPropertyFlags memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    uint16_t layerCount = 1;
    uint16_t mipCount = 1;
    //VkImageLayout dstLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    VkImageCreateFlags flags = 0;
    //VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    //VkImageLayout finalLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	VmaAllocationCreateFlags allocationFlags = 0;

    bool resized = false;

    void init(uint16_t aWidth, uint16_t aHeight);

    void destroy();

    void transitionLayout(uint8_t threadIndex, uint8_t imageIndex, VkImageLayout srcLayout, VkImageLayout dstLayout);

    void resize(uint16_t width, uint16_t height);

    void generateMipmaps(uint8_t threadIndex);

    void copyToFromImage(uint8_t threadIndex, Vulkan_Image& source);

    void blitToFromImage(uint8_t threadIndex, Vulkan_Image& source);

    void fillWithData(uint8_t threadIndex, void* data, uint16_t bytesPerPixel); //TODO: query this information at runtime
};

} //namespace lv

#endif
