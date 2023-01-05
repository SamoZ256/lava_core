#include "lvcore/core/buffer_helper.hpp"

#include "lvcore/core/device.hpp"
#include "lvcore/core/swap_chain.hpp"
#include "lvcore/core/allocator.hpp"

namespace lv {

VmaAllocation BufferHelper::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage, VkBuffer &buffer, VmaAllocationInfo* allocInfo, VkMemoryPropertyFlags properties, VmaAllocationCreateFlags allocationFlags) {
  VkBufferCreateInfo bufferInfo{};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  
  VmaAllocationCreateInfo allocCreateInfo = {};
  allocCreateInfo.usage = memoryUsage;
  allocCreateInfo.requiredFlags = properties;
  allocCreateInfo.flags = allocationFlags;
  
  VmaAllocation allocation;
  VK_CHECK_RESULT(vmaCreateBuffer(g_allocator->allocator, &bufferInfo, &allocCreateInfo, &buffer, &allocation, allocInfo))

  return allocation;
}

void BufferHelper::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
  VkCommandBuffer commandBuffer = g_device->beginSingleTimeCommands();

  VkBufferCopy copyRegion{};
  copyRegion.srcOffset = 0;  // Optional
  copyRegion.dstOffset = 0;  // Optional
  copyRegion.size = size;
  vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

  g_device->endSingleTimeCommands(commandBuffer);
}

void BufferHelper::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount, uint8_t arrayLayer, VkImageAspectFlagBits aspectMask) {
  VkCommandBuffer commandBuffer = g_device->beginSingleTimeCommands();

  VkBufferImageCopy region{};
  region.bufferOffset = 0;
  region.bufferRowLength = 0;
  region.bufferImageHeight = 0;

  region.imageSubresource.aspectMask = aspectMask;
  region.imageSubresource.mipLevel = 0;
  region.imageSubresource.baseArrayLayer = arrayLayer;
  region.imageSubresource.layerCount = layerCount;

  region.imageOffset = {0, 0, 0};
  region.imageExtent = {width, height, 1};

  vkCmdCopyBufferToImage(
      commandBuffer,
      buffer,
      image,
      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      1,
      &region);
  g_device->endSingleTimeCommands(commandBuffer);
}

void BufferHelper::copyImage(VkImage srcImage, VkImage dstImage, uint32_t width, uint32_t height, uint32_t srcLayerCount, uint8_t srcArrayLayer, uint32_t dstLayerCount, uint8_t dstArrayLayer , VkImageAspectFlagBits aspectMask) {
  VkCommandBuffer commandBuffer = g_device->beginSingleTimeCommands();

  VkImageCopy region{};
  region.srcOffset = {0, 0, 0};
  
  region.srcSubresource.aspectMask = aspectMask;
  region.srcSubresource.mipLevel = 0;
  region.srcSubresource.baseArrayLayer = srcArrayLayer;
  region.srcSubresource.layerCount = srcLayerCount;

  region.dstSubresource.aspectMask = aspectMask;
  region.dstSubresource.mipLevel = 0;
  region.dstSubresource.baseArrayLayer = dstArrayLayer;
  region.dstSubresource.layerCount = dstLayerCount;

  region.dstOffset = {0, 0, 0};
  region.extent = {width, height, 1};

  vkCmdCopyImage(
      commandBuffer,
      srcImage,
      VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
      dstImage,
      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      1,
      &region);
  g_device->endSingleTimeCommands(commandBuffer);
}

} //namespace lv
