#ifndef LV_INSTANCE_H
#define LV_INSTANCE_H

#include <string>
#include <vector>
#include <cstring>
#include <iostream>
#include <set>
#include <unordered_set>

#define LVND_BACKEND_VULKAN
#include "lvnd/lvnd.h"

namespace lv {

struct InstanceCreateInfo {
    const char* applicationName;
    uint32_t applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    uint32_t vulkanVersion = VK_API_VERSION_1_1;
    bool enableValidationLayers = false;
};

class Instance {
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

    Instance(InstanceCreateInfo& createInfo);

    void destroy();

    void setupDebugMessenger();
    bool checkValidationLayerSupport();
    void hasGflwRequiredInstanceExtensions();
    std::vector<const char *> getRequiredExtensions();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

    const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
};

extern Instance* g_instance;

//Implementation
//class Inst { public: static Instance* g_instance; };

} //namespace lv

#endif
