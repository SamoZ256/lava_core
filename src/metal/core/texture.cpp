#include "lvcore/core/texture.hpp"

#include <stb/stb_image.h>

namespace lv {

void Texture::init() {
    image.frameCount = 1;
    image.format = MTL::PixelFormatRGBA8Unorm_sRGB;
    image.usage = MTL::TextureUsageShaderRead;
    image.storageMode = MTL::StorageModeManaged;
    image.init(width, height);
    image.images[0]->replaceRegion(MTL::Region(0, 0, 0, width, height, 1), 0, textureData, width * 4);

    sampler.filter = filter;
    sampler.addressMode = MTL::SamplerAddressModeRepeat;
    sampler.init();
}

void Texture::destroy() {
    image.destroy();
    sampler.destroy();
}

void Texture::load(const char* aFilename) {
    filename = std::string(aFilename);

    int aWidth, aHeight, nbChannels;
    textureData = (void*)stbi_load(aFilename, &aWidth, &aHeight, &nbChannels, STBI_rgb_alpha);
    width = aWidth;
    height = aHeight;

    if (!textureData) {
        throw std::runtime_error(("Failed to load image '" + filename + "'").c_str());
    }
}

} //namespace lv
