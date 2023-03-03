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

    LvFormat format;
    LvImageUsageFlags usage = 0;
    LvImageAspectFlags aspectMask = LV_IMAGE_ASPECT_COLOR_BIT;
    LvImageViewType viewType = LV_IMAGE_VIEW_TYPE_2D;
    LvMemoryType memoryType = LV_MEMORY_TYPE_PRIVATE;
    LvMemoryAllocationCreateFlags memoryAllocationFlags = 0;
    uint16_t layerCount = 1;
    uint16_t mipCount = 1;

    static Metal_ShaderModule blitComputeModule;
    static Metal_ComputePipeline blitComputePipeline;

    void init(uint16_t aWidth, uint16_t aHeight);

    void initFromFile(const char* filename, bool generateMipmaps = false);

    void destroy() { for (uint8_t i = 0; i < frameCount; i++) images[i]->release(); }

    void copyDataTo(uint8_t threadIndex, void* data);

    void transitionLayout(uint8_t threadIndex, uint8_t imageIndex, LvImageLayout srcLayout, LvImageLayout dstLayout) {}

    void _bind(uint16_t index, LvShaderStageFlags shaderStage = LV_SHADER_STAGE_FRAGMENT_BIT);

    void generateMipmaps(uint8_t threadIndex);

    void copyToFromImage(uint8_t threadIndex, Metal_Image& source);

    void blitToFromImage(uint8_t threadIndex, Metal_Image& source);
};

} //namespace lv

#endif
