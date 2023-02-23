#ifndef LV_OPENGL_SAMPLER_H
#define LV_OPENGL_SAMPLER_H

#include "enums.hpp"

#include "image_view.hpp"

namespace lv {

class OpenGL_Sampler {
public:
    GLenum sampler;

    LvFilter filter = LV_FILTER_NEAREST;
    LvSamplerAddressMode addressMode = LV_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    LvBool compareEnable = LV_FALSE;
    LvCompareOp compareOp = LV_COMPARE_OP_LESS;
    float minLod = 0.0f;
    float maxLod = 0.0f;

    void init();

    void destroy() { glDeleteSamplers(1, &sampler); }

    OpenGL_ImageInfo descriptorInfo(OpenGL_ImageView& imageView, LvImageLayout imageLayout = LV_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
};

} //namespace lv

#endif
