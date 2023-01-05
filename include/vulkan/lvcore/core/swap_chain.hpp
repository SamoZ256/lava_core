#ifndef LV_SWAP_CHAIN_H
#define LV_SWAP_CHAIN_H

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

namespace lv {

struct SwapChainCreateInfo {
	LvndWindow* window;
  	bool vsyncEnabled = true;
	bool clearAttachment = false;
};

class SwapChain {
public:
	bool vsyncEnabled = true;

	uint32_t imageIndex = 0;
	uint8_t crntFrame = 0;

	VkFormat depthFormat;

	LvndWindow* _window;

	SwapChain(SwapChainCreateInfo& createInfo);

	void init(LvndWindow* window);

	void resize(LvndWindow* window);
	
	void destroy();

	void destroyToResize();

	void renderAndPresent();

	SwapChain(const SwapChain &) = delete;
	void operator=(const SwapChain &) = delete;

	//VkFramebuffer getFramebuffer(int index) { return swapChainFramebuffers[index]; }
	//VkRenderPass getRenderPass() { return renderPass; }
	//VkImageView getImageView(int index) { return swapChainImageViews[index]; }
	size_t imageCount() { return image.images.size(); }
	//VkFormat getSwapChainImageFormat() { return swapChainImageFormat; }
	VkExtent2D getSwapChainExtent() { return swapChainExtent; }
	uint32_t width() { return swapChainExtent.width; }
	uint32_t height() { return swapChainExtent.height; }

	VkCommandBuffer& getActiveCommandBuffer() { return activeFramebuffer->commandBuffers[imageIndex]; }

	float extentAspectRatio() {
		return static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height);
	}

	void acquireNextImage(/*uint32_t *imageIndex*/);
	VkResult submitCommandBuffers(const VkCommandBuffer *buffers/*, uint32_t *imageIndex*/);

	VkFormat findDepthFormat();

	void createSwapChain();
	void createImageViews();
	//void createDepthResources();
	void createRenderPass();
	void createFramebuffers();
	void createSyncObjects();

	// Helper functions
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

	static void createCommandBuffers(std::vector<VkCommandBuffer>& commandBuffers);

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

	RenderPass renderPass;
	Framebuffer framebuffer;
	Image image;
	ImageView imageView;
	//Image depthImage;
	//ImageView depthImageView;

	//std::vector<VkImage> swapChainImages;
	//std::vector<VkImageView> swapChainImageViews;

	VkExtent2D windowExtent;

	VkSwapchainKHR swapChain;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;

	//Current active
	Framebuffer* activeFramebuffer;
};

extern SwapChain* g_swapChain;

//Implementation
//class Swap { public: static SwapChain* g_swapChain; };

} //namespace lv

#endif
