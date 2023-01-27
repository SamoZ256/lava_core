#ifndef LV_IMAGE_VIEW_H
#define LV_IMAGE_VIEW_H

#include "image.hpp"

namespace lv {

struct ImageInfo {
    std::vector<VkDescriptorImageInfo> infos;
    VkDescriptorType descriptorType;
};

class ImageView {
public:
    uint8_t frameCount = 0;

    std::vector<VkImageView> imageViews;
    Image* image;

    VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D;
    uint8_t baseLayer = 0;
    int8_t layerCount = -1;
    uint8_t baseMip = 0;
    int8_t mipCount = -1;

    void init(Image* aImage);

    void destroy();

    ImageInfo descriptorInfo(VkDescriptorType descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
};

} //namespace lv

#endif
