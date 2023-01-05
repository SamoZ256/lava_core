#include "lvcore/core/sampler.hpp"

#include "lvcore/core/device.hpp"

namespace lv {

void Sampler::init(float maxLod) {
    //float maxLod = float(imageView->mipCount == -1 ? imageView->image->mipCount : imageView->mipCount) - 1.0f;
    ImageHelper::createImageSampler(sampler, filter, (VkSamplerAddressMode)addressMode, 0.0f, maxLod);
}

void Sampler::destroy() {
    vkDestroySampler(g_device->device(), sampler, nullptr);
}

} //namespace lv
