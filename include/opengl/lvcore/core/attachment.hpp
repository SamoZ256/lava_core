#ifndef LV_OPENGL_ATTACHMENT_H
#define LV_OPENGL_ATTACHMENT_H

#include "enums.hpp"

#include "image_view.hpp"

namespace lv {

struct OpenGL_RenderPassAttachment {
    GLenum format;
    int8_t index = -1;
    LvAttachmentLoadOp loadOp = LV_ATTACHMENT_LOAD_OP_DONT_CARE;
    LvAttachmentStoreOp storeOp = LV_ATTACHMENT_STORE_OP_STORE;
    bool blendEnable = false;
    GLenum srcRgbBlendFactor = GL_SRC_ALPHA;
    GLenum dstRgbBlendFactor = GL_ONE_MINUS_SRC_ALPHA;
    GLenum rgbBlendOp = GL_FUNC_ADD;
    GLenum srcAlphaBlendFactor = GL_ONE;
    GLenum dstAlphaBlendFactor = GL_ONE;
    GLenum alphaBlendOp = GL_MAX;
    LvImageLayout initialLayout = LV_IMAGE_LAYOUT_UNDEFINED;
    LvImageLayout finalLayout = LV_IMAGE_LAYOUT_UNDEFINED;
};

struct OpenGL_SubpassAttachment {
    int8_t index = -1;
    LvImageLayout layout = LV_IMAGE_LAYOUT_UNDEFINED;
};

struct OpenGL_FramebufferAttachment {
    OpenGL_ImageView* imageView = nullptr;
    int8_t index = -1;
};

} //namespace lv

#endif
