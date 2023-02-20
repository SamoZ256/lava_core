#ifndef LV_METAL_IMAGE_VIEW_H
#define LV_METAL_IMAGE_VIEW_H

#include "image.hpp"

namespace lv {

struct Metal_ImageInfo {
    std::vector<MTL::Texture*> imageViews;
    MTL::SamplerState* sampler;
    LvDescriptorType descriptorType;
};

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

    Metal_ImageInfo descriptorInfo(LvDescriptorType descriptorType = LV_DESCRIPTOR_TYPE_SAMPLED_IMAGE, LvImageLayout imageLayout = LV_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
};

} //namespace lv

#endif
