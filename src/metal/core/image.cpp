#include "lvcore/core/image.hpp"

#include "lvcore/core/device.hpp"
#include "lvcore/core/swap_chain.hpp"

namespace lv {

void Image::init(uint16_t width, uint16_t height) {
    MTL::TextureDescriptor* textureDesc = MTL::TextureDescriptor::alloc()->init();
    textureDesc->setWidth(width);
    textureDesc->setHeight(height);
    textureDesc->setPixelFormat(format);
    textureDesc->setTextureType(viewType);
    textureDesc->setStorageMode(storageMode);
    textureDesc->setUsage(usage);
    textureDesc->setArrayLength(NS::UInteger(layerCount / ((viewType == MTL::TextureTypeCube || viewType == MTL::TextureTypeCubeArray) ? 6 : 1)));
    textureDesc->setMipmapLevelCount(mipCount);

    if (frameCount == -1) frameCount = g_swapChain->maxFramesInFlight;
    images.resize(frameCount);
    for (uint8_t i = 0; i < frameCount; i++)
        images[i] = g_device->device->newTexture(textureDesc);

    textureDesc->release();
}

void Image::bind(uint16_t index) {
    g_swapChain->activeFramebuffer->encoder->setFragmentTexture(images[std::min(g_swapChain->crntFrame, uint8_t(images.size() - 1))], index);
}

} //namespace lv
