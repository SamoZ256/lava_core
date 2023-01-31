#include "vulkan/lvcore/core/image_view.hpp"

#include "vulkan/lvcore/core/device.hpp"
#include "vulkan/lvcore/core/swap_chain.hpp"

namespace lv {

void Vulkan_ImageView::init(Vulkan_Image* aImage) {
    if (frameCount == 0) frameCount = aImage->frameCount;

    image = aImage;

    if (viewType == VK_IMAGE_VIEW_TYPE_MAX_ENUM) viewType = image->viewType;

    if (layerCount == 0)
        layerCount = image->layerCount;
    if (mipCount == 0)
        mipCount = image->mipCount;

    imageViews.resize(frameCount);
    //std::cout << (int)baseLayer << " : " << (int)trueLayerCount << std::endl;

    for (uint8_t i = 0; i < frameCount; i++) {
        //Image view
        Vulkan_ImageHelper::createImageView(imageViews[i], image->images[i], image->format, image->aspectMask, viewType, baseLayer, layerCount, baseMip, mipCount);
        //std::cout << (int)trueMipCount << " : " << (int)baseMip << std::endl;
    }
}

void Vulkan_ImageView::destroy() {
    for (uint8_t i = 0; i < imageViews.size(); i++) {
        vkDestroyImageView(g_vulkan_device->device(), imageViews[i], nullptr);
    }
}

Vulkan_ImageInfo Vulkan_ImageView::descriptorInfo(VkDescriptorType descriptorType, VkImageLayout imageLayout) {
    Vulkan_ImageInfo info;
    info.infos.resize(imageViews.size());
    for (uint8_t i = 0; i < info.infos.size(); i++) {
        info.infos[i].imageLayout = imageLayout;
        info.infos[i].imageView = imageViews[i];
        info.infos[i].sampler = VK_NULL_HANDLE;
    }
    info.descriptorType = descriptorType;

    return info;
}

} //namespace lv
