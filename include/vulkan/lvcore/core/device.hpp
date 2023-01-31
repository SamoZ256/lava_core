#ifndef LV_VULKAN_DEVICE_H
#define LV_VULKAN_DEVICE_H

// std lib headers
#include <string>
#include <vector>
#include <cstring>
#include <iostream>
#include <set>
#include <unordered_set>

//#include "VMA.cpp"

#include "instance.hpp"

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
};

class Vulkan_Device {
public:
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

    VkCommandPool getCommandPool() { return commandPool; }
    VkDevice device() { return device_; }
    VkSurfaceKHR surface() { return surface_; }
    VkQueue graphicsQueue() { return graphicsQueue_; }
    VkQueue presentQueue() { return presentQueue_; }

    SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(physicalDevice); }
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(physicalDevice); }
    VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    // Buffer Helper Functions
    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    void createImageWithInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory);

    VkPhysicalDeviceProperties properties;

    void pickPhysicalDevice();
    void createLogicalDevice();
    void createCommandPool();

    // helper functions
    bool isDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkCommandPool commandPool;

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
