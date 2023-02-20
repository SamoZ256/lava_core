#include "vulkan/lvcore/core/semaphore.hpp"

#include "vulkan/lvcore/core/device.hpp"
#include "vulkan/lvcore/core/swap_chain.hpp"

namespace lv {

void Vulkan_Semaphore::init() {
    if (frameCount == -1) frameCount = g_vulkan_swapChain->maxFramesInFlight;

    VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    semaphores.resize(frameCount);
    for (uint8_t i = 0; i < frameCount; i++)
        VK_CHECK_RESULT(vkCreateSemaphore(g_vulkan_device->device(), &semaphoreInfo, nullptr, &semaphores[i]));
}

void Vulkan_Semaphore::destroy() {
    for (uint8_t i = 0; i < frameCount; i++)
        vkDestroySemaphore(g_vulkan_device->device(), semaphores[i], nullptr);
}

} //namespace lv
