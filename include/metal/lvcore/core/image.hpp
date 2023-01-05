#ifndef LV_IMAGE_H
#define LV_IMAGE_H

#include <vector>

#include <Metal/Metal.hpp>

namespace lv {

class Image {
public:
    int8_t frameCount = -1;

    std::vector<MTL::Texture*> images;

    MTL::PixelFormat format;
    MTL::TextureUsage usage = 0;
    MTL::StorageMode storageMode = MTL::StorageModePrivate;
    MTL::TextureType viewType = MTL::TextureType2D;
    uint16_t layerCount = 1;
    uint16_t mipCount = 1;
    MTL::LoadAction loadOp = MTL::LoadActionDontCare;

    void init(uint16_t width, uint16_t height);

    void destroy() { for (uint8_t i = 0; i < frameCount; i++) images[i]->release(); }

    void bind(uint16_t index);
};

} //namespace lv

#endif
