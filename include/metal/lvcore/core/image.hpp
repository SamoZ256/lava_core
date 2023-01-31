#ifndef LV_METAL_IMAGE_H
#define LV_METAL_IMAGE_H

#include <vector>

#include <Metal/Metal.hpp>

#include "enums.hpp"

namespace lv {

class Metal_Image {
public:
    uint8_t frameCount = 0;

    std::vector<MTL::Texture*> images;

    MTL::PixelFormat format;
    MTL::TextureUsage usage = 0;
    LvImageAspect aspectMask = 0;
    MTL::TextureType viewType = MTL::TextureType2D;
    MTL::StorageMode memoryProperties = MTL::StorageModePrivate;
    uint16_t layerCount = 1;
    uint16_t mipCount = 1;

    void init(uint16_t width, uint16_t height);

    void destroy() { for (uint8_t i = 0; i < frameCount; i++) images[i]->release(); }

    void bind(uint16_t index, LvShaderStage shaderStage = LV_SHADER_STAGE_FRAGMENT_BIT);

    void generateMipmaps();
};

} //namespace lv

#endif
