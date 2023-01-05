#ifndef LV_SAMPLER_H
#define LV_SAMPLER_H

#include <iostream>

#include "image_view.hpp"

namespace lv {

struct ImageInfo {
    std::vector<VkDescriptorImageInfo> infos;
};

class Sampler {
public:
    VkSampler sampler;

    VkFilter filter = VK_FILTER_NEAREST;
    VkSamplerAddressMode addressMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

    void init(float maxLod = 0.0f);

    void destroy();

    ImageInfo descriptorInfo(ImageView& imageView, VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        ImageInfo info;
        info.infos.resize(imageView.imageViews.size());
        for (uint8_t i = 0; i < info.infos.size(); i++) {
            info.infos[i].imageLayout = imageLayout;
            info.infos[i].imageView = imageView.imageViews[i];
            info.infos[i].sampler = sampler;
        }

        return info;
    }
};

} //namespace lv

#endif
