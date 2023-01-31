#include "vulkan/lvcore/core/instance.hpp"

#include "vulkan/lvcore/core/common.hpp"

namespace lv {

//Implementation
Vulkan_Instance* g_vulkan_instance = nullptr;

// local callback functions
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	} else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

Vulkan_Instance::Vulkan_Instance(Vulkan_InstanceCreateInfo& createInfo) {
	enableValidationLayers = createInfo.enableValidationLayers;
    if (enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("Validation layers requested, but not available");
	}

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = createInfo.applicationName;
	appInfo.applicationVersion = createInfo.applicationVersion;
	appInfo.pEngineName = "Lava Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 1, 0);
	appInfo.apiVersion = createInfo.vulkanVersion;

	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &appInfo;
#ifdef __APPLE__
	instanceCreateInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

	auto extensions = getRequiredExtensions();
	instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	if (enableValidationLayers) {
		instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();

		populateDebugMessengerCreateInfo(debugCreateInfo);
		instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
	} else {
		instanceCreateInfo.enabledLayerCount = 0;
		instanceCreateInfo.pNext = nullptr;
	}

	VK_CHECK_RESULT(vkCreateInstance(&instanceCreateInfo, nullptr, &instance))

	hasGflwRequiredInstanceExtensions();
	setupDebugMessenger();

	g_vulkan_instance = this;
}

void Vulkan_Instance::destroy() {
    std::cout << "Destroying instance" << std::endl;
	if (enableValidationLayers) {
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}

	vkDestroyInstance(instance, nullptr);
    std::cout << "Destroyed instance" << std::endl;
}

void Vulkan_Instance::setupDebugMessenger() {
	if (!enableValidationLayers) return;
	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);
	VK_CHECK_RESULT(CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger))
}

bool Vulkan_Instance::checkValidationLayerSupport() {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char *layerName : validationLayers) {
		bool layerFound = false;

		for (const auto &layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

void Vulkan_Instance::hasGflwRequiredInstanceExtensions() {
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	std::cout << "available extensions:" << std::endl;
	std::unordered_set<std::string> available;
	for (const auto &extension : extensions) {
		std::cout << "\t" << extension.extensionName << std::endl;
		available.insert(extension.extensionName);
	}

	std::cout << "required extensions:" << std::endl;
	auto requiredExtensions = getRequiredExtensions();
	for (const auto &required : requiredExtensions) {
		std::cout << "\t" << required << std::endl;
		if (available.find(required) == available.end()) {
			throw std::runtime_error("Missing required glfw extension");
		}
	}
}

void Vulkan_Instance::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
								VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
							VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
							VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
	createInfo.pUserData = nullptr;  // Optional
}

std::vector<const char *> Vulkan_Instance::getRequiredExtensions() {
	uint16_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = lvndVulkanGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
	/*
	for (uint8_t i = 0; i < extensions.size(); i++) {
		std::cout << "GLFWextension[" << (int)i << "]: " << extensions[i] << std::endl;
	}
	*/
#ifdef __APPLE__
	extensions.push_back("VK_KHR_portability_enumeration");
#endif

	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

} //namespace lv
