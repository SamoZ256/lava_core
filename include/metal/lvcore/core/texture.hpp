#ifndef LV_METAL_TEXTURE_H
#define LV_METAL_TEXTURE_H

#include <string>
#include <stdexcept>

#include "image.hpp"
#include "sampler.hpp"

namespace lv {

class Metal_Texture {
public:
    Metal_Image image;
    Metal_ImageView imageView;
    Metal_Sampler sampler;

    std::string filename = "";

    MTL::PixelFormat format = MTL::PixelFormatRGBA8Unorm;
    MTL::SamplerMinMagFilter filter = MTL::SamplerMinMagFilterLinear;

    bool generateMipmaps = false;

    uint16_t width, height;
    void* textureData;

    void init(uint8_t threadIndex);

    void destroy();

    void load(const char* aFilename);
};

} //namespace lv

#endif
