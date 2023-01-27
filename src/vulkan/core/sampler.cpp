#include "lvcore/core/sampler.hpp"

#include "lvcore/core/device.hpp"

namespace lv {

void Sampler::init() {
    //float maxLod = float(imageView->mipCount == -1 ? imageView->image->mipCount : imageView->mipCount) - 1.0f;
    ImageHelper::createImageSampler(sampler, filter, (VkSamplerAddressMode)addressMode, (compareEnable ? compareOp : VK_COMPARE_OP_MAX_ENUM), minLod, maxLod);
}

void Sampler::destroy() {
    vkDestroySampler(g_device->device(), sampler, nullptr);
}

ImageInfo Sampler::descriptorInfo(ImageView& imageView, VkImageLayout imageLayout) {
    ImageInfo info;
    info.infos.resize(imageView.imageViews.size());
    for (uint8_t i = 0; i < info.infos.size(); i++) {
        info.infos[i].imageLayout = imageLayout;
        info.infos[i].imageView = imageView.imageViews[i];
        info.infos[i].sampler = sampler;
    }
    info.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

    return info;
}

} //namespace lv
