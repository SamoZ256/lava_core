#include "lvcore/core/image.hpp"

#include "lvcore/core/allocator.hpp"
#include "lvcore/core/device.hpp"
#include "lvcore/core/swap_chain.hpp"

namespace lv {

void Image::init(uint16_t aWidth, uint16_t aHeight) {
    if (frameCount == 0) frameCount = g_swapChain->maxFramesInFlight;

    width = aWidth;
    height = aHeight;

    if (usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT || usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)
        allocationFlags |= VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
    images.resize(frameCount);
    allocations.resize(frameCount);
    for (uint8_t i = 0; i < frameCount; i++) {
        //Creating image
        allocations[i] = ImageHelper::createImage((uint16_t)width, (uint16_t)height, format, VK_IMAGE_TILING_OPTIMAL, usage, VMA_MEMORY_USAGE_GPU_ONLY, images[i], nullptr, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, layerCount, mipCount, allocationFlags, flags);
        //ImageBuffer::transitionImageLayout(images[i], format, VK_IMAGE_LAYOUT_UNDEFINED, dstLayout, aspectMask);
    }
}

void Image::destroy() {
    for (uint8_t i = 0; i < frameCount; i++)
        vmaDestroyImage(g_allocator->allocator, images[i], allocations[i]);
}

void Image::transitionLayout(VkImageLayout srcLayout, VkImageLayout dstLayout) {
    for (uint8_t i = 0; i < images.size(); i++) {
        ImageHelper::transitionImageLayout(images[i], format, srcLayout, dstLayout, aspectMask, layerCount, mipCount);
    }
}

void Image::resize(uint16_t width, uint16_t height) {
    destroy();
    init(width, height);
}

void Image::generateMipmaps() {
    VkCommandBuffer commandBuffer = g_device->beginSingleTimeCommands();

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

    g_device->endSingleTimeCommands(commandBuffer);
}

} //namespace lv
