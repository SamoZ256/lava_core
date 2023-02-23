#ifndef LV_VULKAN_INSTANCE_H
#define LV_VULKAN_INSTANCE_H

#include <string>
#include <vector>
#include <cstring>
#include <iostream>
#include <set>
#include <unordered_set>

#include <vulkan/vulkan.h>

#define LV_MAKE_VERSION VK_API_MAKE_VERSION

#define LV_VULKAN_VERSION_1_0 VK_API_VERSION_1_0
#define LV_VULKAN_VERSION_1_1 VK_API_VERSION_1_1
#define LV_VULKAN_VERSION_1_2 VK_API_VERSION_1_2
#define LV_VULKAN_VERSION_1_3 VK_API_VERSION_1_3

namespace lv {

struct Vulkan_InstanceCreateInfo {
    const char* applicationName;
    uint32_t applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    uint32_t vulkanVersion = VK_API_VERSION_1_0;
    bool validationEnable = false;
};

class Vulkan_Instance {
public:
    uint32_t vulkanVersion;
    bool validationEnable;

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;

    Vulkan_Instance(Vulkan_InstanceCreateInfo& createInfo);

    void destroy();

    void setupDebugMessenger();
    bool checkValidationLayerSupport();
    void hasGflwRequiredInstanceExtensions();
    std::vector<const char *> getRequiredExtensions();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

    const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
};

extern Vulkan_Instance* g_vulkan_instance;

} //namespace lv

#endif
