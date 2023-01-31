#include "vulkan/lvcore/core/allocator.hpp"

#include "vulkan/lvcore/core/common.hpp"

#include "vulkan/lvcore/core/device.hpp"

namespace lv {

//Implementation
Vulkan_Allocator* g_vulkan_allocator = nullptr;

Vulkan_Allocator::Vulkan_Allocator(Vulkan_AllocatorCreateInfo& createInfo) {
    /*
    VmaVulkanFunctions vulkanFunctions = {};
    vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
    vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;
    */
    
    VmaAllocatorCreateInfo allocatorCreateInfo = {};
    allocatorCreateInfo.vulkanApiVersion = createInfo.vulkanVersion;
    allocatorCreateInfo.physicalDevice = g_vulkan_device->physicalDevice;
    allocatorCreateInfo.device = g_vulkan_device->device();
    allocatorCreateInfo.instance = g_vulkan_instance->instance;
    //allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;
    
    VK_CHECK_RESULT(vmaCreateAllocator(&allocatorCreateInfo, &allocator));

    g_vulkan_allocator = this;
}

void Vulkan_Allocator::destroy() {
    vmaDestroyAllocator(allocator);
}

} //namespace lv
