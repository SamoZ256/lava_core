#include "lvcore/core/allocator.hpp"

#include "lvcore/core/common.hpp"

#include "lvcore/core/device.hpp"

namespace lv {

//Implementation
Allocator* g_allocator = nullptr;

Allocator::Allocator(AllocatorCreateInfo& createInfo) {
    /*
    VmaVulkanFunctions vulkanFunctions = {};
    vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
    vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;
    */
    
    VmaAllocatorCreateInfo allocatorCreateInfo = {};
    allocatorCreateInfo.vulkanApiVersion = createInfo.vulkanVersion;
    allocatorCreateInfo.physicalDevice = g_device->physicalDevice;
    allocatorCreateInfo.device = g_device->device();
    allocatorCreateInfo.instance = g_instance->instance;
    //allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;
    
    VK_CHECK_RESULT(vmaCreateAllocator(&allocatorCreateInfo, &allocator));

    g_allocator = this;
}

void Allocator::destroy() {
    std::cout << "Destroying allocator" << std::endl;
    vmaDestroyAllocator(allocator);
    std::cout << "Destroyed allocator" << std::endl;
}

} //namespace lv
