#ifndef LV_METAL_IMAGE_VIEW_H
#define LV_METAL_IMAGE_VIEW_H

#include "image.hpp"

namespace lv {

class Metal_ImageView {
public:
    uint8_t frameCount = 0;

    std::vector<MTL::Texture*> imageViews;
    Metal_Image* image;

    MTL::TextureType viewType = MTL::TextureType(10);
    uint16_t baseLayer = 0;
    uint16_t layerCount = 0;
    uint16_t baseMip = 0;
    uint16_t mipCount = 0;

    void init(Metal_Image* aImage);

    void destroy() {}
};

} //namespace lv

#endif
