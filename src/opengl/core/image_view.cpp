#include "opengl/lvcore/core/image_view.hpp"

#include <iostream>

namespace lv {

void OpenGL_ImageView::init(OpenGL_Image* aImage) {
    image = aImage;

    if (viewType == LV_IMAGE_VIEW_TYPE_UNDEFINED) viewType = image->viewType;

    if (layerCount == 0)
        layerCount = image->layerCount;
    else if (layerCount != 1)
        throw std::invalid_argument("ImageView::init: layerCount (" + std::to_string(layerCount) + ") must be <= 1 in case of OpenGL backend");
    if (mipCount == 0)
        mipCount = image->mipCount;
    else if (mipCount != 1)
        throw std::invalid_argument("ImageView::init: mipCount (" + std::to_string(mipCount) + ") must be <= 1 in case of OpenGL backend");
}

OpenGL_ImageInfo OpenGL_ImageView::descriptorInfo(LvDescriptorType descriptorType, LvImageLayout imageLayout) {
    OpenGL_ImageInfo info;
    info.image = image->image;
    info.descriptorType = descriptorType;

    return info;
}

} //namespace lv
