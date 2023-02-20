#ifndef LV_METAL_SAMPLER_H
#define LV_METAL_SAMPLER_H

#include "enums.hpp"

#include "image_view.hpp"

namespace lv {

class Metal_Sampler {
public:
    MTL::SamplerState* sampler;
    MTL::SamplerMinMagFilter filter = MTL::SamplerMinMagFilterNearest;
    MTL::SamplerAddressMode addressMode = MTL::SamplerAddressModeClampToEdge;
    unsigned int compareEnable = false;
    MTL::CompareFunction compareOp = MTL::CompareFunctionLess;
    float minLod = 0.0f;
    float maxLod = 0.0f;

    void init();

    void destroy() { sampler->release(); }

    void bind(uint16_t index, LvShaderStage shaderStage = LV_SHADER_STAGE_FRAGMENT_BIT);

    Metal_ImageInfo descriptorInfo(Metal_ImageView& imageView, LvImageLayout imageLayout = LV_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
};

} //namespace lv

#endif
