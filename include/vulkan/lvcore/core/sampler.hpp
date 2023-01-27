#ifndef LV_SAMPLER_H
#define LV_SAMPLER_H

#include <iostream>

#include "image_view.hpp"

namespace lv {

class Sampler {
public:
    VkSampler sampler;

    VkFilter filter = VK_FILTER_NEAREST;
    VkSamplerAddressMode addressMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    unsigned int compareEnable = VK_FALSE;
    VkCompareOp compareOp = VK_COMPARE_OP_LESS;
    float minLod = 0.0f;
    float maxLod = 0.0f;

    void init();

    void destroy();

    ImageInfo descriptorInfo(ImageView& imageView, VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
};

} //namespace lv

#endif
