#ifndef LV_VULKAN_DEVICE_H
#define LV_VULKAN_DEVICE_H

#include <string>
#include <vector>
#include <cstring>
#include <iostream>
#include <set>
#include <unordered_set>

#include <vk_mem_alloc.h>

#define LVND_BACKEND_VULKAN
#include "lvnd/lvnd.h"

#include "instance.hpp"

#include "lvcore/threading/thread_pool.hpp"

namespace lv {

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices {
	uint32_t graphicsFamily;
	uint32_t presentFamily;
	bool graphicsFamilyHasValue = false;
	bool presentFamilyHasValue = false;
	bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
};

struct Vulkan_DeviceCreateInfo {
    LvndWindow* window;
    ThreadPool* threadPool;
};

class Vulkan_Device {
public:
    uint8_t maxThreadCount;

    Vulkan_Device(Vulkan_DeviceCreateInfo& createInfo);

    void destroy();

    // Not copyable or movable
    /*
    Device(const Device &) = delete;
    void operator=(const Device &) = delete;
    Device(Device &&) = delete;
    Device &operator=(Device &&) = delete;
    */

    void waitIdle() { vkDeviceWaitIdle(device_); }

    VkCommandPool getCommandPool(uint8_t threadIndex) { return commandPools[threadIndex]; }
    VkDevice device() { return device_; }
    VkSurfaceKHR surface() { return surface_; }
    VkQueue graphicsQueue() { return graphicsQueue_; }
    VkQueue presentQueue() { return presentQueue_; }

    SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(physicalDevice); }
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(physicalDevice); }
    VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    // Buffer Helper Functions
    VkCommandBuffer beginSingleTimeCommands(uint8_t threadIndex);
    void endSingleTimeCommands(uint8_t threadIndex, VkCommandBuffer commandBuffer);

	//void createCommandBuffers(uint8_t threadIndex, std::vector<VkCommandBuffer>& commandBuffers);

    //void createImageWithInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory);

    VkPhysicalDeviceProperties properties;

    void pickPhysicalDevice();
    void createLogicalDevice();
    void createCommandPool(uint8_t index);

    // helper functions
    bool isDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    std::vector<VkCommandPool> commandPools;

    VmaAllocator allocator;

    VkDevice device_;
    VkSurfaceKHR surface_;
    VkQueue graphicsQueue_;
    VkQueue presentQueue_;

    const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_KHR_MAINTENANCE1_EXTENSION_NAME, VK_EXT_SHADER_VIEWPORT_INDEX_LAYER_EXTENSION_NAME/*, "VK_KHR_portability_subset"*/};
};

extern Vulkan_Device* g_vulkan_device;

//Implementation
//class Dev { public: static Device* g_metal_device; };

} //namespace lv

#endif
