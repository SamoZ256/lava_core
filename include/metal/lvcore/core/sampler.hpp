#ifndef LV_METAL_SAMPLER_H
#define LV_METAL_SAMPLER_H

#include "enums.hpp"

#include "image_view.hpp"

namespace lv {

class Metal_Sampler {
public:
    MTL::SamplerState* sampler;

    LvFilter filter = LV_FILTER_NEAREST;
    LvSamplerAddressMode addressMode = LV_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    LvBool compareEnable = LV_FALSE;
    LvCompareOp compareOp = LV_COMPARE_OP_LESS;
    float minLod = 0.0f;
    float maxLod = 0.0f;

    void init();

    void destroy() { sampler->release(); }

    void _bind(uint16_t index, LvShaderStageFlags shaderStage = LV_SHADER_STAGE_FRAGMENT_BIT);

    Metal_ImageInfo descriptorInfo(Metal_ImageView& imageView, LvImageLayout imageLayout = LV_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
};

} //namespace lv

#endif
