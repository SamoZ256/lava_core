#ifndef LV_VULKAN_IMAGE_VIEW_H
#define LV_VULKAN_IMAGE_VIEW_H

#include "image.hpp"

namespace lv {

struct Vulkan_ImageInfo {
    std::vector<VkDescriptorImageInfo> infos;
    VkDescriptorType descriptorType;
};

class Vulkan_ImageView {
public:
    uint8_t frameCount = 0;

    std::vector<VkImageView> imageViews;
    Vulkan_Image* image;

    VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_MAX_ENUM;
    uint16_t baseLayer = 0;
    uint16_t layerCount = 0;
    uint16_t baseMip = 0;
    uint16_t mipCount = 0;

    void init(Vulkan_Image* aImage);

    void destroy();

    Vulkan_ImageInfo descriptorInfo(VkDescriptorType descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
};

} //namespace lv

#endif
