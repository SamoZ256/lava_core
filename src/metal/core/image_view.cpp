#include "metal/lvcore/core/image_view.hpp"

#include "metal/lvcore/core/swap_chain.hpp"

#include <iostream>

namespace lv {

void Metal_ImageView::init(Metal_Image* aImage) {
    if (frameCount == 0) frameCount = aImage->frameCount;

    image = aImage;

    if (viewType == 10) viewType = image->viewType;

    if (layerCount == 0)
        layerCount = image->layerCount;
    if (mipCount == 0)
        mipCount = image->mipCount;
    
    imageViews.resize(frameCount);

    for (uint8_t i = 0; i < frameCount; i++) {
        //std::cout << "Mip range: " << (int)baseMip << " : " << (int)mipCount << std::endl;
        imageViews[i] = image->images[i]->newTextureView(image->format, viewType, NS::Range::Make(baseMip, mipCount), NS::Range::Make(baseLayer, layerCount));
    }
}

Metal_ImageInfo Metal_ImageView::descriptorInfo(LvDescriptorType descriptorType, LvImageLayout imageLayout) {
    Metal_ImageInfo info;
    info.imageViews.resize(frameCount);
    for (uint8_t i = 0; i < frameCount; i++)
        info.imageViews[i] = imageViews[i];
    info.descriptorType = descriptorType;

    return info;
}

} //namespace lv
