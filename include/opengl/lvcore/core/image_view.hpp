#ifndef LV_OPENGL_IMAGE_VIEW_H
#define LV_OPENGL_IMAGE_VIEW_H

#include "enums.hpp"

#include "image.hpp"

namespace lv {

struct OpenGL_ImageInfo {
    GLuint image;
    GLuint sampler;
    LvImageViewType viewType;
    LvDescriptorType descriptorType;
};

class OpenGL_ImageView {
public:
    uint8_t frameCount = 0;

    OpenGL_Image* image;

    LvImageViewType viewType = LV_IMAGE_VIEW_TYPE_UNDEFINED;
    uint16_t baseLayer = 0;
    uint16_t layerCount = 0;
    uint16_t baseMip = 0;
    uint16_t mipCount = 0;

    void init(OpenGL_Image* aImage);

    void destroy() {}

    OpenGL_ImageInfo descriptorInfo(LvDescriptorType descriptorType = LV_DESCRIPTOR_TYPE_SAMPLED_IMAGE, LvImageLayout imageLayout = LV_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
};

} //namespace lv

#endif
