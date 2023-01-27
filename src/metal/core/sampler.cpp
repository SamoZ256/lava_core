#include "lvcore/core/sampler.hpp"

#include <string>

#include "lvcore/core/device.hpp"

namespace lv {

void Sampler::init() {
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

    sampler = g_device->device->newSamplerState(samplerDesc);
}

void Sampler::bind(uint16_t index, LvShaderStage shaderStage) {
    switch (shaderStage) {
        case LV_SHADER_STAGE_FRAGMENT_BIT:
            g_swapChain->activeRenderEncoder->setFragmentSamplerState(sampler, index);
            break;
        case LV_SHADER_STAGE_COMPUTE_BIT:
            g_swapChain->activeComputeEncoder->setSamplerState(sampler, index);
            break;
        default:
            throw std::invalid_argument("Sampler::bind: invalid shader stage '" + std::to_string(shaderStage) + "'");
    }
}

} //namespace lv
