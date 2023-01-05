#include "lvcore/core/sampler.hpp"

#include "lvcore/core/device.hpp"

namespace lv {

void Sampler::init(float maxLod) {
    MTL::SamplerDescriptor* samplerDesc = MTL::SamplerDescriptor::alloc()->init();
    samplerDesc->setMinFilter(MTL::SamplerMinMagFilterNearest);
    samplerDesc->setMagFilter(filter);
    samplerDesc->setRAddressMode(addressMode);
    samplerDesc->setSAddressMode(addressMode);
    samplerDesc->setTAddressMode(addressMode);
    samplerDesc->setLodMaxClamp(maxLod);
    if (maxLod != 0.0f)
        samplerDesc->setMipFilter(MTL::SamplerMipFilterLinear);

    sampler = g_device->device->newSamplerState(samplerDesc);
}

} //namespace lv
