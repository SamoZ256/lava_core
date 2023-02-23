#include "metal/lvcore/core/image.hpp"

#include <string>

#include <stb/stb_image.h>

#include "metal/lvcore/core/device.hpp"
#include "metal/lvcore/core/swap_chain.hpp"
#include "metal/lvcore/core/buffer.hpp"

#include "lvcore/filesystem/filesystem.hpp"

namespace lv {

Metal_ShaderModule Metal_Image::blitComputeModule = Metal_ShaderModule();
Metal_ComputePipeline Metal_Image::blitComputePipeline = Metal_ComputePipeline();

void Metal_Image::init(uint16_t aWidth, uint16_t aHeight) {
    if (frameCount == 0) frameCount = g_metal_swapChain->maxFramesInFlight;

    width = aWidth;
    height = aHeight;

    MTL::TextureDescriptor* textureDesc = MTL::TextureDescriptor::alloc()->init();
    textureDesc->setWidth(width);
    textureDesc->setHeight(height);
    textureDesc->setPixelFormat(format);
    textureDesc->setTextureType(viewType);
    textureDesc->setStorageMode(memoryType);
    textureDesc->setUsage(usage);
    textureDesc->setArrayLength(NS::UInteger(layerCount / ((viewType == MTL::TextureTypeCube || viewType == MTL::TextureTypeCubeArray) ? 6 : 1)));
    textureDesc->setMipmapLevelCount(mipCount);

    images.resize(frameCount);
    for (uint8_t i = 0; i < frameCount; i++)
        images[i] = g_metal_device->device->newTexture(textureDesc);

    textureDesc->release();
}

void Metal_Image::initFromFile(const char* filename) {
    int aWidth, aHeight, nbChannels;
    void* data = (void*)stbi_load(filename, &aWidth, &aHeight, &nbChannels, STBI_rgb_alpha);
    width = aWidth;
    height = aHeight;

    if (!data) {
        throw std::runtime_error(("Failed to load image '" + std::string(filename) + "'").c_str());
    }

    init(width, height);
    copyDataTo(0, data);

    stbi_image_free(data);
}

void Metal_Image::copyDataTo(uint8_t threadIndex, void* data) {
    size_t size = width * height * 4; //TODO: remove this hardcoding
    MTL::Buffer* stagingBuffer = g_metal_device->device->newBuffer(size, MTL::ResourceStorageModeShared);
    memcpy(stagingBuffer->contents(), data, size);

    for (uint8_t i = 0; i < frameCount; i++)
        Metal_Buffer::copyBufferToImage(stagingBuffer, images[i], width, height);

    stagingBuffer->release();
}

void Metal_Image::_bind(uint16_t index, LvShaderStageFlags shaderStage) {
    switch (shaderStage) {
        case LV_SHADER_STAGE_FRAGMENT_BIT:
            g_metal_swapChain->activeRenderEncoder->setFragmentTexture(images[std::min(g_metal_swapChain->crntFrame, uint8_t(images.size() - 1))], index);
            break;
        case LV_SHADER_STAGE_COMPUTE_BIT:
            g_metal_swapChain->activeComputeEncoder->setTexture(images[std::min(g_metal_swapChain->crntFrame, uint8_t(images.size() - 1))], index);
            break;
        default:
            throw std::invalid_argument("Image::bind: invalid shader stage '" + std::to_string(shaderStage) + "'");
    }
}

void Metal_Image::generateMipmaps(uint8_t threadIndex) {
    MTL::CommandBuffer* commandBuffer = g_metal_device->commandQueue->commandBuffer();
    MTL::BlitCommandEncoder* blitCommandEncoder = commandBuffer->blitCommandEncoder();

    for (uint8_t i = 0; i < images.size(); i++)
        blitCommandEncoder->generateMipmaps(images[i]);

    blitCommandEncoder->endEncoding();
    commandBuffer->commit();
}

void Metal_Image::copyToFromImage(uint8_t threadIndex, Metal_Image& source) {
    MTL::CommandBuffer* commandBuffer = g_metal_swapChain->getActiveCommandBuffer();//g_metal_device->commandQueue->commandBuffer();
    MTL::BlitCommandEncoder* blitCommandEncoder = commandBuffer->blitCommandEncoder();

    blitCommandEncoder->copyFromTexture(source.images[std::min(g_metal_swapChain->crntFrame, uint8_t(source.frameCount - 1))], images[std::min(g_metal_swapChain->crntFrame, uint8_t(frameCount - 1))]);

    blitCommandEncoder->endEncoding();
    //commandBuffer->commit();
}

void Metal_Image::blitToFromImage(uint8_t threadIndex, Metal_Image& source) {
    if (blitComputePipeline.computePipeline == nullptr) {
        Metal_ShaderModuleCreateInfo blitComputeModuleCI;
        blitComputeModuleCI.source = readFile("/Users/samuliak/Documents/lava_core/src/metal/core/blit.metallib"); //TODO: load this file independently of path

        blitComputeModule.init(blitComputeModuleCI);

        Metal_ComputePipelineCreateInfo blitComputePipelineCI;
        blitComputePipelineCI.computeShaderModule = &blitComputeModule;

        blitComputePipeline.init(blitComputePipelineCI);
    }

    blitComputePipeline.bind();

    source._bind(0, LV_SHADER_STAGE_COMPUTE_BIT);
    _bind(1, LV_SHADER_STAGE_COMPUTE_BIT);

    blitComputePipeline.dispatch(64, 64, 1,
                                (width + 64 - 1) / 64, (height + 64 - 1) / 64, 1);
}

} //namespace lv
