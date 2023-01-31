#ifndef LV_VULKAN_INSTANCE_H
#define LV_VULKAN_INSTANCE_H

#include <string>
#include <vector>
#include <cstring>
#include <iostream>
#include <set>
#include <unordered_set>

#define LVND_BACKEND_VULKAN
#include "lvnd/lvnd.h"

namespace lv {

struct Vulkan_InstanceCreateInfo {
    const char* applicationName;
    uint32_t applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    uint32_t vulkanVersion = VK_API_VERSION_1_1;
    bool enableValidationLayers = false;
};

class Vulkan_Instance {
public:
/*
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
   	const bool enableValidationLayers = true;
#endif
*/
    bool enableValidationLayers;

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

//Implementation
//class Inst { public: static Instance* g_instance; };

} //namespace lv

#endif
