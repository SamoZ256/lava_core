#ifndef LV_METAL_ATTACHMENT_H
#define LV_METAL_ATTACHMENT_H

#include "enums.hpp"

#include "image_view.hpp"

namespace lv {

struct Metal_RenderPassAttachment {
    MTL::PixelFormat format;
    int8_t index = -1;
    MTL::LoadAction loadOp = MTL::LoadActionDontCare;
    MTL::StoreAction storeOp = MTL::StoreActionStore;
    bool blendEnable = false;
    MTL::BlendFactor srcRgbBlendFactor = MTL::BlendFactorSourceAlpha;
    MTL::BlendFactor dstRgbBlendFactor = MTL::BlendFactorOneMinusSourceAlpha;
    MTL::BlendOperation rgbBlendOp = MTL::BlendOperationAdd;
    MTL::BlendFactor srcAlphaBlendFactor = MTL::BlendFactorOne;
    MTL::BlendFactor dstAlphaBlendFactor = MTL::BlendFactorOne;
    MTL::BlendOperation alphaBlendOp = MTL::BlendOperationMax;
    LvImageLayout initialLayout = LV_IMAGE_LAYOUT_UNDEFINED;
    LvImageLayout finalLayout = LV_IMAGE_LAYOUT_UNDEFINED;
};

struct Metal_SubpassAttachment {
    int8_t index = -1;
    LvImageLayout layout = LV_IMAGE_LAYOUT_UNDEFINED;
};

struct Metal_FramebufferAttachment {
    Metal_ImageView* imageView = nullptr;
    int8_t index = -1;
};

} //namespace lv

#endif
