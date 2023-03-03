#include "vulkan/lvcore/core/swap_chain.hpp"

namespace lv {

//Implementation
Vulkan_SwapChain* g_vulkan_swapChain = nullptr;

Vulkan_SwapChain::Vulkan_SwapChain(Vulkan_SwapChainCreateInfo& createInfo) {
	g_vulkan_swapChain = this;

	vsyncEnabled = createInfo.vsyncEnabled;
	maxFramesInFlight = createInfo.maxFramesInFlight;
	_window = createInfo.window;
	//image.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	if (createInfo.clearAttachment)
		loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthFormat = findDepthFormat();

	subpass.addColorAttachment({
		.index = 0,
		.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	});

	renderPass.addSubpass(&subpass);

	std::array<VkSubpassDependency, 2> dependencies{};
	/*
	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].srcAccessMask = 0;
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependencies[0].dstSubpass = 0;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	*/
	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].srcAccessMask = 0;
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependencies[0].dstSubpass = 0;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	dependencies[1].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[1].dstSubpass = 0;
	dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].srcAccessMask = 0;  // or VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[1].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	renderPass.dependencies.push_back(dependencies[0]);
	renderPass.dependencies.push_back(dependencies[1]);
	
	renderPass.addAttachment({
		.format = VK_FORMAT_B8G8R8A8_SRGB,
		.index = 0,
		.loadOp = loadOp,
		.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
	});

	init(createInfo.window);
	createSyncObjects();

	//activeRenderPass = &renderPass.renderPass;
	//activeCommandBuffers = &framebuffer.commandBuffers;
	//activeFramebuffer = &framebuffer;
}

void Vulkan_SwapChain::init(LvndWindow* window) {
	uint16_t width, height;
	lvndGetWindowSize(window, &width, &height);
	windowExtent = {width, height};
	//findDepthFormat();
	createSwapChain();
	createImageViews();
	renderPass.init();
	//createDepthResources();
	framebuffer.frameCount = imageCount();
	framebuffer.init(&renderPass);
	commandBuffer.frameCount = imageCount();
	commandBuffer.init();

	//window.width = width();
	//window.height = height();
}

void Vulkan_SwapChain::resize(LvndWindow* window) {
	/*
	float xscale, yscale;
    lvndWindowGetFramebufferScale(window, &xscale, &yscale);

    uint16_t width, height;
    lvndWindowGetFramebufferSize(window, &width, &height);
	*/

    //window.width = width / xscale;// * xscale;
    //window.height = height / yscale;// * yscale;

	destroyToResize();

	init(window);
}

void Vulkan_SwapChain::destroyToResize() {
	imageView.destroy();

	oldSwapChain = swapChain;

	framebuffer.destroy();
	commandBuffer.destroy();
	renderPass.destroy();
}

void Vulkan_SwapChain::destroy() {
	destroyToResize();

	renderFinishedSemaphore.destroy();
	imageAvailableSemaphore.destroy();
	for (size_t i = 0; i < maxFramesInFlight; i++) {
		vkDestroyFence(g_vulkan_device->device(), inFlightFences[i], nullptr);
	}
	
	vkDestroySwapchainKHR(g_vulkan_device->device(), swapChain, nullptr);
}

void Vulkan_SwapChain::renderAndPresent() {
	VK_CHECK_RESULT(submitCommandBuffers(&commandBuffer.commandBuffers[imageIndex]));
}

void Vulkan_SwapChain::acquireNextImage(/*uint32_t *imageIndex*/) {
	vkWaitForFences(g_vulkan_device->device(), 1, &inFlightFences[crntFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

	VK_CHECK_RESULT(vkAcquireNextImageKHR(g_vulkan_device->device(), swapChain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphore.semaphores[crntFrame], VK_NULL_HANDLE, &imageIndex));
}

VkResult Vulkan_SwapChain::submitCommandBuffers(const VkCommandBuffer* buffers/*, uint32_t *imageIndex*/) {
	if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
		vkWaitForFences(g_vulkan_device->device(), 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	}
	imagesInFlight[imageIndex] = inFlightFences[crntFrame];

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {imageAvailableSemaphore.semaphores[crntFrame]};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = buffers;

	//VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[crntFrame]};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &renderFinishedSemaphore.semaphores[crntFrame];

	vkResetFences(g_vulkan_device->device(), 1, &inFlightFences[crntFrame]);
	if (vkQueueSubmit(g_vulkan_device->graphicsQueue(), 1, &submitInfo, inFlightFences[crntFrame]) !=
		VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &renderFinishedSemaphore.semaphores[crntFrame];

	VkSwapchainKHR swapChains[] = {swapChain};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &imageIndex;

	auto result = vkQueuePresentKHR(g_vulkan_device->presentQueue(), &presentInfo);

	crntFrame = (crntFrame + 1) % maxFramesInFlight;

	return result;
}

void Vulkan_SwapChain::createSwapChain() {
	SwapChainSupportDetails swapChainSupport = g_vulkan_device->getSwapChainSupport();

	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

	uint32_t imageCount = std::max(maxFramesInFlight, (uint8_t)swapChainSupport.capabilities.minImageCount);
	//std::cout << "Min image count: " << (int)swapChainSupport.capabilities.minImageCount << std::endl;
	//std::cout << "Max image count: " << (int)swapChainSupport.capabilities.maxImageCount << std::endl;
	if (swapChainSupport.capabilities.maxImageCount > 0 &&
		imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}
	//std::cout << "Image count: " << (int)imageCount << std::endl;

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = g_vulkan_device->surface();

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = g_vulkan_device->findPhysicalQueueFamilies();
	uint32_t queueFamilyIndices[] = {indices.graphicsFamily, indices.presentFamily};

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	} else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;      // Optional
		createInfo.pQueueFamilyIndices = nullptr;  // Optional
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = oldSwapChain;

	VK_CHECK_RESULT(vkCreateSwapchainKHR(g_vulkan_device->device(), &createInfo, nullptr, &swapChain));

	swapChainExtent = extent;

	// we only specified a minimum number of images in the swap chain, so the implementation is
	// allowed to create a swap chain with more. That's why we'll first query the final number of
	// images with vkGetSwapchainImagesKHR, then resize the container and finally call it again to
	// retrieve the handles.
	vkGetSwapchainImagesKHR(g_vulkan_device->device(), swapChain, &imageCount, nullptr);
	maxFramesInFlight = std::min(maxFramesInFlight, (uint8_t)imageCount);
	//std::cout << "Max frames in flight: " << (int)maxFramesInFlight << std::endl;
	image.frameCount = imageCount;
	image.width = swapChainExtent.width;
	image.height = swapChainExtent.height;
	image.images.resize(image.frameCount);
	vkGetSwapchainImagesKHR(g_vulkan_device->device(), swapChain, &imageCount, image.images.data());
	image.format = surfaceFormat.format;
	//image.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	//image.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	image.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

	if (oldSwapChain != VK_NULL_HANDLE)
		vkDestroySwapchainKHR(g_vulkan_device->device(), oldSwapChain, nullptr);
	
	/*
	depthImage.frameCount = imageCount;
	depthImage.usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	depthImage.format = depthFormat;
	depthImage.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	depthImage.init(swapChainExtent.width, swapChainExtent.height);
	depthImageView.frameCount = imageCount;
	depthImageView.init(&depthImage);
	*/

	/*
	images.resize(swapChainImages.size());
	for (uint8_t i = 0; i < images.size(); i++) {
		images[i].images = {swapChainImages[i]};
	}
	*/
}

void Vulkan_SwapChain::createImageViews() {
	imageView.frameCount = image.frameCount;
	imageView.init(&image);
	/*
	imageView.imageViews.resize(imageView.frameCount);
	for (uint8_t i = 0; i < imageView.imageViews.size(); i++) {
		//imageViews[i].init(&images[i]);
		//imageViews[i].imageViews.resize(1);
		ImageBuffer::createImageView(imageView.imageViews[i], image.images[i], image.format);
	}
	*/
	framebuffer.colorAttachments.clear();
	framebuffer.addColorAttachment({
		.imageView = &imageView,
		.index = 0
	});
	//framebuffer.setDepthAttachment({&depthImage, &depthImageView, 1});
	//image.format = swapChainImageFormat;
	/*
	swapChainImageViews.resize(swapChainImages.size());
	for (size_t i = 0; i < swapChainImages.size(); i++) {
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = swapChainImages[i];
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = swapChainImageFormat;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(g_metal_device->device(), &viewInfo, nullptr, &swapChainImageViews[i]) !=
			VK_SUCCESS) {
		throw std::runtime_error("failed to create texture image view!");
		}
	}
	*/
}

/*
void SwapChain::createDepthResources() {
  depthFormat = findDepthFormat();
  VkExtent2D swapChainExtent = getSwapChainExtent();

  depthImages.resize(imageCount());
  depthImageMemorys.resize(imageCount());
  depthImageViews.resize(imageCount());

  for (int i = 0; i < depthImages.size(); i++) {
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = swapChainExtent.width;
    imageInfo.extent.height = swapChainExtent.height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = depthFormat;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.flags = 0;

    g_metal_device->createImageWithInfo(
        imageInfo,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        depthImages[i],
        depthImageMemorys[i]);

    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = depthImages[i];
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = depthFormat;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(g_metal_device->device(), &viewInfo, nullptr, &depthImageViews[i]) != VK_SUCCESS) {
      throw std::runtime_error("failed to create texture image view!");
    }
  }
}
*/

void Vulkan_SwapChain::createSyncObjects() {
	inFlightFences.resize(maxFramesInFlight);
	imagesInFlight.resize(imageCount(), VK_NULL_HANDLE);

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	imageAvailableSemaphore.init();
	renderFinishedSemaphore.init();
	for (size_t i = 0; i < maxFramesInFlight; i++) {
		VK_CHECK_RESULT(vkCreateFence(g_vulkan_device->device(), &fenceInfo, nullptr, &inFlightFences[i]));
	}
}

VkSurfaceFormatKHR Vulkan_SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) {
	for (const auto &availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
			availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR Vulkan_SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) {
	/*
	for (const auto &availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
		std::cout << "Present mode: Mailbox" << std::endl;
		return availablePresentMode;
		}
	}
	*/

	// for (const auto &availablePresentMode : availablePresentModes) {
	//   if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
	//     std::cout << "Present mode: Immediate" << std::endl;
	//     return availablePresentMode;
	//   }
	// }

	return vsyncEnabled ? VK_PRESENT_MODE_FIFO_KHR : VK_PRESENT_MODE_IMMEDIATE_KHR;
}

VkExtent2D Vulkan_SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) {
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	} else {
		VkExtent2D actualExtent = windowExtent;
		actualExtent.width = std::max(
			capabilities.minImageExtent.width,
			std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(
			capabilities.minImageExtent.height,
			std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}

VkFormat Vulkan_SwapChain::findDepthFormat() {
    return g_vulkan_device->findSupportedFormat(
      {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
      VK_IMAGE_TILING_OPTIMAL,
      VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

} //namespace lv
