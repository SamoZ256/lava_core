#ifndef LV_IMAGE_HELPER_H
#define LV_IMAGE_HELPER_H

#include "common.hpp"
#include <vma/vk_mem_alloc.h>

namespace lv {

class ImageHelper {
public:
    //static VkFormat getImageFormat(uint8_t nbChannels);

    static VmaAllocation createImage(uint16_t width, uint16_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VmaMemoryUsage memoryUsage, VkImage& image, VmaAllocationInfo* allocInfo = nullptr, VkMemoryPropertyFlags properties = 0, uint8_t layerCount = 1, uint8_t mipCount = 1, VmaAllocationCreateFlags allocationFlags = 0, VkImageCreateFlags flags = 0);

    static void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, uint8_t layerCount = 1, uint8_t mipCount = 1);

    static void createImageView(VkImageView& imageView, VkImage& image, VkFormat format, VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D, uint8_t baseLayer = 0, uint8_t layerCount = 1, uint8_t baseMip = 0, uint8_t mipCount = 1);

    static void createImageSampler(VkSampler& sampler, VkFilter filter = VK_FILTER_LINEAR, VkSamplerAddressMode addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT, float minLod = 0.0f, float maxLod = 0.0f);
};

} //namespace lv

#endif
