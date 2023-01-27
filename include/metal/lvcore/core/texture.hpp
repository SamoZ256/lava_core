#ifndef LV_TEXTURE_H
#define LV_TEXTURE_H

#include <string>
#include <stdexcept>

#include "image.hpp"
#include "sampler.hpp"

namespace lv {

class Texture {
public:
    Image image;
    Sampler sampler;

    std::string filename = "";

    MTL::SamplerMinMagFilter filter = MTL::SamplerMinMagFilterLinear;

    bool generateMipmaps = false;

    uint16_t width, height;
    void* textureData;

    void init();

    void destroy();

    void load(const char* aFilename);
};

} //namespace lv

#endif
