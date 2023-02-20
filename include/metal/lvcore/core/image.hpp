#ifndef LV_METAL_IMAGE_H
#define LV_METAL_IMAGE_H

#include <vector>

#include "enums.hpp"

#include "compute_pipeline.hpp"

namespace lv {

class Metal_Image {
public:
    uint8_t frameCount = 0;

    std::vector<MTL::Texture*> images;

    uint16_t width, height;

    MTL::PixelFormat format;
    MTL::TextureUsage usage = 0;
    LvImageAspect aspectMask = 0;
    MTL::TextureType viewType = MTL::TextureType2D;
    MTL::StorageMode memoryProperties = MTL::StorageModePrivate;
    uint16_t layerCount = 1;
    uint16_t mipCount = 1;

    static Metal_ShaderModule blitComputeModule;
    static Metal_ComputePipeline blitComputePipeline;

    void init(uint16_t aWidth, uint16_t aHeight);

    void destroy() { for (uint8_t i = 0; i < frameCount; i++) images[i]->release(); }

    void bind(uint16_t index, LvShaderStage shaderStage = LV_SHADER_STAGE_FRAGMENT_BIT);

    void generateMipmaps();

    void copyToFromImage(uint8_t threadIndex, Metal_Image& source);

    void blitToFromImage(uint8_t threadIndex, Metal_Image& source);

    void fillWithData(uint8_t threadIndex, void* data, uint16_t bytesPerPixel); //TODO: query this information at runtime
};

} //namespace lv

#endif
