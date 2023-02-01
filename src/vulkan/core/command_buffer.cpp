#include "vulkan/lvcore/core/command_buffer.hpp"

#include "vulkan/lvcore/core/swap_chain.hpp"

namespace lv {

void Vulkan_CommandBuffer::init() {
    if (frameCount == 0) frameCount = g_vulkan_swapChain->maxFramesInFlight;

    commandBuffers.resize(frameCount);
    g_vulkan_swapChain->createCommandBuffers(commandBuffers);
}

void Vulkan_CommandBuffer::destroy() {
    vkFreeCommandBuffers(g_vulkan_device->device(), g_vulkan_device->commandPool, commandBuffers.size(), commandBuffers.data());
}

void Vulkan_CommandBuffer::bind() {
    uint8_t index = std::min(g_vulkan_swapChain->imageIndex, uint32_t(frameCount - 1));
    index = g_vulkan_swapChain->imageIndex;

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffers[index], &beginInfo));

    g_vulkan_swapChain->activeCommandBuffer = this;
}

void Vulkan_CommandBuffer::unbind() {
    uint8_t index = std::min(g_vulkan_swapChain->imageIndex, uint32_t(frameCount - 1));
    index = g_vulkan_swapChain->imageIndex;
    VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffers[index]));
}

void Vulkan_CommandBuffer::submit() {
    uint8_t index = std::min(g_vulkan_swapChain->imageIndex, uint32_t(frameCount - 1));
    index = g_vulkan_swapChain->imageIndex;

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[index];

    VK_CHECK_RESULT(vkQueueSubmit(g_vulkan_device->graphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE));
}

} //namespace lv
