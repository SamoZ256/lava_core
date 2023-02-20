#include "metal/lvcore/core/texture.hpp"

#include <stb/stb_image.h>

#include "metal/lvcore/core/device.hpp"
#include "metal/lvcore/core/buffer.hpp"

#include <iostream>

namespace lv {

void Metal_Texture::init(uint8_t threadIndex) {
    /*
    size_t size = width * height * 4 * 1;
    MTL::Buffer* stagingBuffer = g_metal_device->device->newBuffer(size, MTL::ResourceStorageModeShared);
    memcpy(stagingBuffer->contents(), textureData, size);
    */
    //stagingBuffer->didModifyRange(NS::Range::Make(0, stagingBuffer->length()));

    image.frameCount = 1;
    image.format = format;
    image.usage = MTL::TextureUsageShaderRead;
    image.memoryProperties = MTL::StorageModePrivate;
    if (generateMipmaps) {
        image.mipCount = std::max(ceil(log2(width)), ceil(log2(height)));
        //std::cout << "MipCount: " << (int)image.mipCount << std::endl;
    }
    //std::cout << (int)width << ", " << (int)height << std::endl;
    image.init(width, height);
    //image.images[0]->replaceRegion(MTL::Region(0, 0, 0, width, height, 1), 0, textureData, width * 4);
    imageView.init(&image);

   //Metal_Buffer::copyBufferToImage(stagingBuffer, image.images[0], width, height);

    //stagingBuffer->release();
    image.fillWithData(threadIndex, textureData, 4); //TODO: remove this hardcoding (this is what prevents using FLOAT32 format for skybox)

    if (generateMipmaps)
        image.generateMipmaps();

    sampler.filter = filter;
    sampler.addressMode = MTL::SamplerAddressModeRepeat;
    //sampler.minLod = 2.0f;
    sampler.maxLod = image.mipCount;
    sampler.init();
}

void Metal_Texture::destroy() {
    image.destroy();
    sampler.destroy();
}

void Metal_Texture::load(const char* aFilename) {
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
