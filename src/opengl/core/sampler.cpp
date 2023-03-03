#include "opengl/lvcore/core/sampler.hpp"

namespace lv {

void OpenGL_Sampler::init() {
    glGenSamplers(1, &sampler);

    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, filter);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_R, addressMode);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, addressMode);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, addressMode);
    if (compareEnable) {
        glSamplerParameteri(sampler, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        glSamplerParameteri(sampler, GL_TEXTURE_COMPARE_FUNC, compareOp);
    }
    glSamplerParameterf(sampler, GL_TEXTURE_MIN_LOD, minLod);
    glSamplerParameterf(sampler, GL_TEXTURE_MAX_LOD, maxLod);
    if (maxLod > 0.0f)
        glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    else
        glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

OpenGL_ImageInfo OpenGL_Sampler::descriptorInfo(OpenGL_ImageView& imageView, LvImageLayout imageLayout) {
    OpenGL_ImageInfo info;
    info.image = imageView.image->image;
    info.sampler = sampler;
    info.viewType = imageView.viewType;
    info.descriptorType = LV_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

    return info;
}

} //namespace lv
