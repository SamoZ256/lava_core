#include "lvcore/core/image_view.hpp"

#include "lvcore/core/device.hpp"

namespace lv {

void ImageView::init(Image* aImage) {
    image = aImage;

    uint8_t trueLayerCount = layerCount == -1 ? image->layerCount : layerCount;
    uint8_t trueMipCount = mipCount == -1 ? image->mipCount : mipCount;

    imageViews.resize(frameCount);
    //std::cout << (int)baseLayer << " : " << (int)trueLayerCount << std::endl;

    for (uint8_t i = 0; i < imageViews.size(); i++) {
        //Image view
        ImageHelper::createImageView(imageViews[i], image->images[i], image->format, image->aspectMask, viewType, baseLayer, trueLayerCount, baseMip, trueMipCount);
        //std::cout << (int)trueMipCount << " : " << (int)baseMip << std::endl;
    }
}

void ImageView::destroy() {
    for (uint8_t i = 0; i < imageViews.size(); i++) {
        vkDestroyImageView(g_device->device(), imageViews[i], nullptr);
    }
}

} //namespace lv