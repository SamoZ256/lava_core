#ifndef LV_VULKAN_SWAP_CHAIN_H
#define LV_VULKAN_SWAP_CHAIN_H

// std
#include <array>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#include "framebuffer.hpp"
#include "device.hpp"
#include "semaphore.hpp"

namespace lv {

struct Vulkan_SwapChainCreateInfo {
	LvndWindow* window;
  	bool vsyncEnabled = true;
	uint8_t maxFramesInFlight = 2;
	bool clearAttachment = false;
};

class Vulkan_SwapChain {
public:
	bool vsyncEnabled = true;

	uint8_t maxFramesInFlight;

	uint32_t imageIndex = 0;
	uint8_t crntFrame = 0;

	VkFormat depthFormat;

	LvndWindow* _window;

	Vulkan_SwapChain(Vulkan_SwapChainCreateInfo& createInfo);

	void init(LvndWindow* window);

	void resize(LvndWindow* window);
	
	void destroy();

	void destroyToResize();

	void renderAndPresent();

	Vulkan_SwapChain(const Vulkan_SwapChain &) = delete;
	void operator=(const Vulkan_SwapChain &) = delete;

	//VkFramebuffer getFramebuffer(int index) { return swapChainFramebuffers[index]; }
	//VkRenderPass getRenderPass() { return renderPass; }
	//VkImageView getImageView(int index) { return swapChainImageViews[index]; }
	size_t imageCount() { return image.images.size(); }
	//VkFormat getSwapChainImageFormat() { return swapChainImageFormat; }
	VkExtent2D getSwapChainExtent() { return swapChainExtent; }
	uint32_t width() { return swapChainExtent.width; }
	uint32_t height() { return swapChainExtent.height; }

	VkCommandBuffer& getActiveCommandBuffer() { return activeCommandBuffer->commandBuffers[std::min(imageIndex, uint32_t(activeCommandBuffer->frameCount - 1))]; }

	float extentAspectRatio() {
		return static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height);
	}

	void acquireNextImage(/*uint32_t *imageIndex*/);
	VkResult submitCommandBuffers(const VkCommandBuffer *buffers/*, uint32_t *imageIndex*/);

	VkFormat findDepthFormat();

	void createSwapChain();
	void createImageViews();
	//void createDepthResources();
	void createSyncObjects();

	// Helper functions
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

	void renderFullscreenTriangle() { vkCmdDraw(getActiveCommandBuffer(), 3, 1, 0, 0); }

	//VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	//std::vector<VkFramebuffer> swapChainFramebuffers;
	//VkRenderPass renderPass;

	//std::vector<VkImage> depthImages;
	//std::vector<VkDeviceMemory> depthImageMemorys;
	//std::vector<VkImageView> depthImageViews;
	//std::vector<VkImage> swapChainImages;
	//std::vector<VkImageView> swapChainImageViews;

	Vulkan_Subpass subpass;
	Vulkan_RenderPass renderPass;
	Vulkan_Framebuffer framebuffer;
	Vulkan_CommandBuffer commandBuffer;
	Vulkan_Image image;
	Vulkan_ImageView imageView;
	//Image depthImage;
	//ImageView depthImageView;

	VkAttachmentLoadOp loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;

	//std::vector<VkImage> swapChainImages;
	//std::vector<VkImageView> swapChainImageViews;

	VkExtent2D windowExtent;

	VkSwapchainKHR swapChain;
	VkSwapchainKHR oldSwapChain = VK_NULL_HANDLE;

	Vulkan_Semaphore imageAvailableSemaphore;
	Vulkan_Semaphore renderFinishedSemaphore;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;

	//Currently active
	Vulkan_CommandBuffer* activeCommandBuffer;
	VkPipelineBindPoint pipelineBindPoint;
};

extern Vulkan_SwapChain* g_vulkan_swapChain;

//Implementation
//class Swap { public: static SwapChain* g_metal_swapChain; };

} //namespace lv

#endif
