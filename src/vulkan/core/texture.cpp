#include "vulkan/lvcore/core/texture.hpp"

#include <cmath>
#include <cstring>

#include <stb/stb_image.h>

#include "vulkan/lvcore/core/allocator.hpp"

namespace lv {

void Vulkan_Texture::init(/*const char* aFilename, TextureData* aTextureData*//*, VkFormat aFormat*/) {
    //format = aFormat;

    VkDeviceSize imageSize = width * height * 4;

    VkBuffer stagingBuffer;

    //VmaAllocationInfo allocInfo;
    VmaAllocation stagingAllocation = Vulkan_BufferHelper::createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, stagingBuffer, nullptr, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    void* data;
    vmaMapMemory(g_vulkan_allocator->allocator, stagingAllocation, &data);
    memcpy(data, textureData, imageSize);
    vmaUnmapMemory(g_vulkan_allocator->allocator, stagingAllocation);

    //Creating image
    //VkFormat format = VK_FORMAT_R8G8B8A8_SRGB;//getImageFormat((uint8_t)nbChannels);
    image.frameCount = 1;
    image.format = VK_FORMAT_R8G8B8A8_SRGB;
    image.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    //image.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    image.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    if (generateMipmaps) {
        image.mipCount = std::max(ceil(log2(width)), ceil(log2(height)));
        //std::cout << "MipCount: " << (int)image.mipCount << std::endl;
    }
    image.init(width, height);
    //allocation = ImageBuffer::createImage((uint16_t)width, (uint16_t)height, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VMA_MEMORY_USAGE_GPU_ONLY, image, nullptr, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    image.transitionLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    //ImageBuffer::transitionImageLayout(image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    Vulkan_BufferHelper::copyBufferToImage(stagingBuffer, image.images[0], width, height);
    //ImageBuffer::transitionImageLayout(image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    image.generateMipmaps();
    
    image.transitionLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    //Image view
    imageView.frameCount = 1;
    imageView.init(&image);
    //ImageBuffer::createImageView(imageView, image, format);

    //Sampler
    sampler.filter = filter;
    //sampler.minLod = 2.0f;
    sampler.maxLod = image.mipCount;
    sampler.addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler.init();
    //ImageBuffer::createImageSampler(sampler, filter);

    //Clean up
    vmaDestroyBuffer(g_vulkan_allocator->allocator, stagingBuffer, stagingAllocation);

    //if (aTextureData == nullptr)
    if (loaded)
        stbi_image_free(textureData);
}

void Vulkan_Texture::load(const char* aFilename) {
    filename = std::string(aFilename);

    //TextureData textureData;
    //if (aTextureData == nullptr) {
    int aWidth, aHeight, nbChannels;
    textureData = (void*)stbi_load(aFilename, &aWidth, &aHeight, &nbChannels, STBI_rgb_alpha);
    width = aWidth;
    height = aHeight;

    if (!textureData) {
        throw std::runtime_error(("Failed to load image '" + filename + "'").c_str());
    }
    loaded = true;
    /*} else {
        textureData = *aTextureData;
        width = textureData.width;
        height = textureData.height;
    }*/
}

void Vulkan_Texture::destroy() {
    /*
    vkDestroySampler(g_metal_device->device(), sampler, nullptr);
    vkDestroyImageView(g_metal_device->device(), imageView, nullptr);
    vmaDestroyImage(g_vulkan_allocator->allocator, image, allocation);
    */
    sampler.destroy();
    imageView.destroy();
    image.destroy();
}

} //namespace lv
