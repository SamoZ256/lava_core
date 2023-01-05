#ifndef LV_SAMPLER_H
#define LV_SAMPLER_H

#include "swap_chain.hpp"

namespace lv {

class Sampler {
public:
    MTL::SamplerState* sampler;
    MTL::SamplerMinMagFilter filter = MTL::SamplerMinMagFilter::SamplerMinMagFilterNearest;
    MTL::SamplerAddressMode addressMode = MTL::SamplerAddressModeClampToEdge;

    void init(float maxLod = 0.0f);

    void destroy() { sampler->release(); }

    void bind(uint16_t index) { g_swapChain->activeFramebuffer->encoder->setFragmentSamplerState(sampler, index); }
};

} //namespace lv

#endif
