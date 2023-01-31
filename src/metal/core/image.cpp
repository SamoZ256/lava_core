#include "metal/lvcore/core/image.hpp"

#include <string>

#include "metal/lvcore/core/device.hpp"
#include "metal/lvcore/core/swap_chain.hpp"

namespace lv {

void Metal_Image::init(uint16_t width, uint16_t height) {
    if (frameCount == 0) frameCount = g_metal_swapChain->maxFramesInFlight;

    MTL::TextureDescriptor* textureDesc = MTL::TextureDescriptor::alloc()->init();
    textureDesc->setWidth(width);
    textureDesc->setHeight(height);
    textureDesc->setPixelFormat(format);
    textureDesc->setTextureType(viewType);
    textureDesc->setStorageMode(memoryProperties);
    textureDesc->setUsage(usage);
    textureDesc->setArrayLength(NS::UInteger(layerCount / ((viewType == MTL::TextureTypeCube || viewType == MTL::TextureTypeCubeArray) ? 6 : 1)));
    textureDesc->setMipmapLevelCount(mipCount);

    images.resize(frameCount);
    for (uint8_t i = 0; i < frameCount; i++)
        images[i] = g_metal_device->device->newTexture(textureDesc);

    textureDesc->release();
}

void Metal_Image::bind(uint16_t index, LvShaderStage shaderStage) {
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

void Metal_Image::generateMipmaps() {
    MTL::CommandBuffer* commandBuffer = g_metal_device->commandQueue->commandBuffer();
    MTL::BlitCommandEncoder* blitCommandEncoder = commandBuffer->blitCommandEncoder();

    for (uint8_t i = 0; i < images.size(); i++)
        blitCommandEncoder->generateMipmaps(images[i]);

    blitCommandEncoder->endEncoding();
    commandBuffer->commit();
}

} //namespace lv
