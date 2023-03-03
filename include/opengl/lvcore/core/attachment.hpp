#ifndef LV_OPENGL_ATTACHMENT_H
#define LV_OPENGL_ATTACHMENT_H

#include "enums.hpp"

#include "image_view.hpp"

namespace lv {

struct OpenGL_ColorBlendAttachment {
    uint8_t index = 0;
    bool blendEnable = LV_FALSE;
    LvBlendFactor srcRgbBlendFactor = LV_BLEND_FACTOR_SRC_ALPHA;
    LvBlendFactor dstRgbBlendFactor = LV_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    LvBlendOp rgbBlendOp = LV_BLEND_OP_ADD;
    LvBlendFactor srcAlphaBlendFactor = LV_BLEND_FACTOR_ONE;
    LvBlendFactor dstAlphaBlendFactor = LV_BLEND_FACTOR_ONE;
    LvBlendOp alphaBlendOp = LV_BLEND_OP_MAX;
};

struct OpenGL_RenderPassAttachment {
    uint8_t index = 0;
    LvFormat format;
    LvAttachmentLoadOp loadOp = LV_ATTACHMENT_LOAD_OP_DONT_CARE;
    LvAttachmentStoreOp storeOp = LV_ATTACHMENT_STORE_OP_STORE;
    LvImageLayout initialLayout = LV_IMAGE_LAYOUT_UNDEFINED;
    LvImageLayout finalLayout = LV_IMAGE_LAYOUT_UNDEFINED;
};

struct OpenGL_SubpassAttachment {
    int8_t index = -1;
    LvImageLayout layout = LV_IMAGE_LAYOUT_UNDEFINED;
};

struct OpenGL_FramebufferAttachment {
    int8_t index = -1;
    OpenGL_ImageView* imageView = nullptr;
    LvClearValue clearValue;
};

} //namespace lv

#endif
