#include "vulkan/lvcore/core/image.hpp"

#include <stb/stb_image.h>

#include "vulkan/lvcore/core/device.hpp"
#include "vulkan/lvcore/core/swap_chain.hpp"

namespace lv {

void Vulkan_Image::init(uint16_t aWidth, uint16_t aHeight) {
    if (frameCount == 0) frameCount = g_vulkan_swapChain->maxFramesInFlight;

    width = aWidth;
    height = aHeight;

    if (usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT || usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)
        memoryAllocationFlags |= VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
    VkImageCreateFlags flags = 0;
    if (viewType == VK_IMAGE_VIEW_TYPE_CUBE || viewType == VK_IMAGE_VIEW_TYPE_CUBE_ARRAY)
        flags |= VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
    
    images.resize(frameCount);
    allocations.resize(frameCount);
    for (uint8_t i = 0; i < frameCount; i++) {
        //Creating image
        allocations[i] = Vulkan_ImageHelper::createImage((uint16_t)width, (uint16_t)height, format, VK_IMAGE_TILING_OPTIMAL, usage, images[i], nullptr, memoryType, layerCount, mipCount, memoryAllocationFlags, flags);
        //ImageBuffer::transitionImageLayout(images[i], format, VK_IMAGE_LAYOUT_UNDEFINED, dstLayout, aspectMask);
    }
}

void Vulkan_Image::initFromFile(const char* filename) {
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

void Vulkan_Image::destroy() {
    for (uint8_t i = 0; i < frameCount; i++)
        vmaDestroyImage(g_vulkan_device->allocator, images[i], allocations[i]);
}

void Vulkan_Image::copyDataTo(uint8_t threadIndex, void* data) {
    VkDeviceSize imageSize = width * height * 4; //TODO: remove this hardcoding

    VkBuffer stagingBuffer;

    VmaAllocation stagingAllocation = Vulkan_BufferHelper::createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, stagingBuffer, nullptr, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    void* mappedData;
    vmaMapMemory(g_vulkan_device->allocator, stagingAllocation, &mappedData);
    memcpy(mappedData, data, imageSize);
    vmaUnmapMemory(g_vulkan_device->allocator, stagingAllocation);

    for (uint8_t i = 0; i < frameCount; i++) {
        transitionLayout(threadIndex, i, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        Vulkan_BufferHelper::copyBufferToImage(threadIndex, stagingBuffer, images[i], width, height);
    }

    //Clean up
    vmaDestroyBuffer(g_vulkan_device->allocator, stagingBuffer, stagingAllocation);
}

void Vulkan_Image::transitionLayout(uint8_t threadIndex, uint8_t imageIndex, VkImageLayout srcLayout, VkImageLayout dstLayout) {
    Vulkan_ImageHelper::transitionImageLayout(threadIndex, images[imageIndex], format, srcLayout, dstLayout, aspectMask, layerCount, mipCount);
}

void Vulkan_Image::resize(uint16_t width, uint16_t height) {
    destroy();
    init(width, height);
}

void Vulkan_Image::generateMipmaps(uint8_t threadIndex) {
    VkCommandBuffer commandBuffer = g_vulkan_device->beginSingleTimeCommands(threadIndex);

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.subresourceRange.levelCount = 1;

    for (uint8_t i = 0; i < images.size(); i++) {
        barrier.image = images[i];

        int32_t mipWidth = width;
        int32_t mipHeight = height;

        for (uint32_t mip = 0; mip < mipCount - 1; mip++) {
            barrier.subresourceRange.baseMipLevel = mip;
            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer,
                VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
                0, nullptr,
                0, nullptr,
                1, &barrier);

            VkImageBlit blit{};
            blit.srcOffsets[0] = {0, 0, 0};
            blit.srcOffsets[1] = {mipWidth, mipHeight, 1};
            blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.srcSubresource.mipLevel = mip;
            blit.srcSubresource.baseArrayLayer = 0;
            blit.srcSubresource.layerCount = 1;

            blit.dstOffsets[0] = {0, 0, 0};
            blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
            blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.dstSubresource.mipLevel = mip + 1;
            blit.dstSubresource.baseArrayLayer = 0;
            blit.dstSubresource.layerCount = 1;

            vkCmdBlitImage(commandBuffer,
                images[i], VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                images[i], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                1, &blit,
                VK_FILTER_LINEAR);
            
            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer,
                VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
                0, nullptr,
                0, nullptr,
                1, &barrier);

            if (mipWidth > 1) mipWidth /= 2;
            if (mipHeight > 1) mipHeight /= 2;
        }
    }

    g_vulkan_device->endSingleTimeCommands(threadIndex, commandBuffer);
}

void Vulkan_Image::copyToFromImage(uint8_t threadIndex, Vulkan_Image& source) {
    throw std::runtime_error("Not implemented yet");
}

void Vulkan_Image::blitToFromImage(uint8_t threadIndex, Vulkan_Image& source) {
    //VkCommandBuffer commandBuffer = g_vulkan_device->beginSingleTimeCommands(threadIndex);
    VkCommandBuffer commandBuffer = g_vulkan_swapChain->getActiveCommandBuffer();

    uint8_t srcIndex = std::min(g_vulkan_swapChain->crntFrame, uint8_t(source.frameCount - 1));
    uint8_t dstIndex = std::min(g_vulkan_swapChain->crntFrame, uint8_t(frameCount - 1));

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask = aspectMask;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;

    barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    barrier.image = source.images[srcIndex];

    vkCmdPipelineBarrier(commandBuffer,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
        0, nullptr,
        0, nullptr,
        1, &barrier);

    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.image = images[dstIndex];

    vkCmdPipelineBarrier(commandBuffer,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
        0, nullptr,
        0, nullptr,
        1, &barrier);

    VkImageBlit blit{};
    blit.srcOffsets[0] = {0, 0, 0};
    blit.srcOffsets[1] = {source.width, source.height, 1};
    blit.srcSubresource.aspectMask = source.aspectMask;
    blit.srcSubresource.mipLevel = 0;
    blit.srcSubresource.baseArrayLayer = 0;
    blit.srcSubresource.layerCount = 1;

    blit.dstOffsets[0] = {0, 0, 0};
    blit.dstOffsets[1] = {width, height, 1 };
    blit.dstSubresource.aspectMask = aspectMask;
    blit.dstSubresource.mipLevel = 0;
    blit.dstSubresource.baseArrayLayer = 0;
    blit.dstSubresource.layerCount = 1;

    vkCmdBlitImage(commandBuffer,
        source.images[srcIndex], VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        images[dstIndex], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1, &blit,
        VK_FILTER_NEAREST);
    
    /*
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    barrier.image = images[dstIndex];

    vkCmdPipelineBarrier(commandBuffer,
        VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
        0, nullptr,
        0, nullptr,
        1, &barrier);
    */

    //g_vulkan_device->endSingleTimeCommands(threadIndex, commandBuffer);
}

} //namespace lv
