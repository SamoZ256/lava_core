#include "metal/lvcore/core/sampler.hpp"

#include <string>

#include "metal/lvcore/core/device.hpp"
#include "metal/lvcore/core/swap_chain.hpp"

namespace lv {

void Metal_Sampler::init() {
    MTL::SamplerDescriptor* samplerDesc = MTL::SamplerDescriptor::alloc()->init();
    samplerDesc->setMagFilter(filter);
    samplerDesc->setRAddressMode(addressMode);
    samplerDesc->setSAddressMode(addressMode);
    samplerDesc->setTAddressMode(addressMode);
    if (compareEnable)
        samplerDesc->setCompareFunction(compareOp);
    samplerDesc->setLodMinClamp(minLod);
    samplerDesc->setLodMaxClamp(maxLod);
    if (maxLod > 0.0f) {
        samplerDesc->setMipFilter(MTL::SamplerMipFilterLinear);
        samplerDesc->setMinFilter(MTL::SamplerMinMagFilterLinear);
        //std::cout << "Sampler lod: " << maxLod << std::endl;
    }

    sampler = g_metal_device->device->newSamplerState(samplerDesc);
}

void Metal_Sampler::_bind(uint16_t index, LvShaderStageFlags shaderStage) {
    switch (shaderStage) {
        case LV_SHADER_STAGE_FRAGMENT_BIT:
            g_metal_swapChain->activeRenderEncoder->setFragmentSamplerState(sampler, index);
            break;
        case LV_SHADER_STAGE_COMPUTE_BIT:
            g_metal_swapChain->activeComputeEncoder->setSamplerState(sampler, index);
            break;
        default:
            throw std::invalid_argument("Sampler::bind: invalid shader stage '" + std::to_string(shaderStage) + "'");
    }
}

Metal_ImageInfo Metal_Sampler::descriptorInfo(Metal_ImageView& imageView, LvImageLayout imageLayout) {
    Metal_ImageInfo info;
    info.imageViews.resize(imageView.frameCount);
    for (uint8_t i = 0; i < imageView.frameCount; i++)
        info.imageViews[i] = imageView.imageViews[i];
    info.sampler = sampler;
    info.descriptorType = LV_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

    return info;
}

} //namespace lv
