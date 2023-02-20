#include "vulkan/lvcore/core/command_buffer.hpp"

#include "vulkan/lvcore/core/swap_chain.hpp"

namespace lv {

void Vulkan_CommandBuffer::init() {
    if (frameCount == 0) frameCount = g_vulkan_swapChain->maxFramesInFlight;

    VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = g_vulkan_device->getCommandPool(threadIndex);
	allocInfo.commandBufferCount = frameCount;

    commandBuffers.resize(frameCount);
	VK_CHECK_RESULT(vkAllocateCommandBuffers(g_vulkan_device->device(), &allocInfo, commandBuffers.data()));
}

void Vulkan_CommandBuffer::destroy() {
    vkFreeCommandBuffers(g_vulkan_device->device(), g_vulkan_device->commandPools[threadIndex], commandBuffers.size(), commandBuffers.data());
}

void Vulkan_CommandBuffer::bind(VkCommandBufferUsageFlags usage) {
    uint8_t index = std::min(g_vulkan_swapChain->imageIndex, uint32_t(frameCount - 1));
    index = g_vulkan_swapChain->imageIndex;

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = usage;

    VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffers[index], &beginInfo));

    g_vulkan_swapChain->activeCommandBuffer = this;
}

void Vulkan_CommandBuffer::unbind() {
    uint8_t index = std::min(g_vulkan_swapChain->imageIndex, uint32_t(frameCount - 1));
    index = g_vulkan_swapChain->imageIndex;
    VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffers[index]));
}

void Vulkan_CommandBuffer::submit(Vulkan_Semaphore* waitSemaphore, Vulkan_Semaphore* signalSemaphore) {
    uint8_t index = std::min(g_vulkan_swapChain->imageIndex, uint32_t(frameCount - 1));
    index = g_vulkan_swapChain->imageIndex;

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[index];
    if (waitSemaphore != nullptr) {
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &waitSemaphore->semaphores[index];
    }
    if (signalSemaphore != nullptr) {
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &signalSemaphore->semaphores[index];
    }

    VK_CHECK_RESULT(vkQueueSubmit(g_vulkan_device->graphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE));
}

} //namespace lv
