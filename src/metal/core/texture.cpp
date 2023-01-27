#include "lvcore/core/texture.hpp"

#include <stb/stb_image.h>

#include "lvcore/core/device.hpp"
#include "lvcore/core/buffer.hpp"

#include <iostream>

namespace lv {

void Texture::init() {
    size_t size = width * height * 4 * 1;
    MTL::Buffer* stagingBuffer = g_device->device->newBuffer(size, MTL::ResourceStorageModeShared);
    memcpy(stagingBuffer->contents(), textureData, size);
    //stagingBuffer->didModifyRange(NS::Range::Make(0, stagingBuffer->length()));

    image.frameCount = 1;
    image.format = MTL::PixelFormatRGBA8Unorm_sRGB;
    image.usage = MTL::TextureUsageShaderRead;
    image.storageMode = MTL::StorageModePrivate;
    if (generateMipmaps) {
        image.mipCount = std::max(ceil(log2(width)), ceil(log2(height)));
        //std::cout << "MipCount: " << (int)image.mipCount << std::endl;
    }
    //std::cout << (int)width << ", " << (int)height << std::endl;
    image.init(width, height);
    //image.images[0]->replaceRegion(MTL::Region(0, 0, 0, width, height, 1), 0, textureData, width * 4);

    Buffer::copyBufferToImage(stagingBuffer, image.images[0], width, height);

    stagingBuffer->release();

    if (generateMipmaps)
        image.generateMipmaps();

    sampler.filter = filter;
    sampler.addressMode = MTL::SamplerAddressModeRepeat;
    //sampler.minLod = 2.0f;
    sampler.maxLod = image.mipCount;
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
