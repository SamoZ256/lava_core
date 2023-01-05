#ifndef LV_IMAGE_VIEW_H
#define LV_IMAGE_VIEW_H

#include "image.hpp"

namespace lv {

class ImageView {
public:
    uint8_t frameCount = MAX_FRAMES_IN_FLIGHT;

    std::vector<VkImageView> imageViews;
    Image* image;

    VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D;
    uint8_t baseLayer = 0;
    int8_t layerCount = -1;
    uint8_t baseMip = 0;
    int8_t mipCount = -1;

    void init(Image* aImage);

    void destroy();
};

} //namespace lv

#endif
