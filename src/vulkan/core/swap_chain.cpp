#include "lvcore/core/swap_chain.hpp"

namespace lv {

//Implementation
SwapChain* g_swapChain = nullptr;

SwapChain::SwapChain(SwapChainCreateInfo& createInfo) {
	vsyncEnabled = createInfo.vsyncEnabled;
	_window = createInfo.window;
	if (createInfo.clearAttachment)
		image.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthFormat = findDepthFormat();
	init(createInfo.window);
	createSyncObjects();

	g_swapChain = this;

	//activeRenderPass = &renderPass.renderPass;
	//activeCommandBuffers = &framebuffer.commandBuffers;
	//activeFramebuffer = &framebuffer;
}

void SwapChain::init(LvndWindow* window) {
	uint16_t width, height;
	lvndGetWindowSize(window, &width, &height);
	windowExtent = {width, height};
	//findDepthFormat();
	createSwapChain();
	createImageViews();
	createRenderPass();
	//createDepthResources();
	createFramebuffers();

	//window.width = width();
	//window.height = height();
}

void SwapChain::resize(LvndWindow* window) {
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

void SwapChain::destroyToResize() {
	/*
	for (auto imageView : swapChainImageViews) {
		vkDestroyImageView(g_device->device(), imageView, nullptr);
	}
	swapChainImageViews.clear();
	*/
	//for (auto& imageView : imageViews)
	imageView.destroy();

	if (swapChain != nullptr) {
		vkDestroySwapchainKHR(g_device->device(), swapChain, nullptr);
		swapChain = nullptr;
	}

	//image.images.clear();

	/*
	for (int i = 0; i < depthImages.size(); i++) {
		vkDestroyImageView(g_device->device(), depthImageViews[i], nullptr);
		vkDestroyImage(g_device->device(), depthImages[i], nullptr);
		vkFreeMemory(g_device->device(), depthImageMemorys[i], nullptr);
	}
	*/

	/*
	for (auto framebuffer : swapChainFramebuffers) {
		vkDestroyFramebuffer(g_device->device(), framebuffer, nullptr);
	}
	*/
	framebuffer.destroy();

	//vkDestroyRenderPass(g_device->device(), renderPass, nullptr);
	renderPass.destroy();
}

void SwapChain::destroy() {
    std::cout << "Destroying swap chain" << std::endl;
	destroyToResize();

	// cleanup synchronization objects
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(g_device->device(), renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(g_device->device(), imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(g_device->device(), inFlightFences[i], nullptr);
	}
    std::cout << "Destroyed swap chain" << std::endl;
}

void SwapChain::renderAndPresent() {
	VK_CHECK_RESULT(submitCommandBuffers(&framebuffer.commandBuffers[imageIndex]))
}

void SwapChain::acquireNextImage(/*uint32_t *imageIndex*/) {
	vkWaitForFences(
		g_device->device(),
		1,
		&inFlightFences[crntFrame],
		VK_TRUE,
		std::numeric_limits<uint64_t>::max());

	VK_CHECK_RESULT(vkAcquireNextImageKHR(g_device->device(), swapChain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphores[crntFrame], VK_NULL_HANDLE, &imageIndex))
}

VkResult SwapChain::submitCommandBuffers(const VkCommandBuffer *buffers/*, uint32_t *imageIndex*/) {
	if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
		vkWaitForFences(g_device->device(), 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	}
	imagesInFlight[imageIndex] = inFlightFences[crntFrame];

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[crntFrame]};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = buffers;

	VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[crntFrame]};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(g_device->device(), 1, &inFlightFences[crntFrame]);
	if (vkQueueSubmit(g_device->graphicsQueue(), 1, &submitInfo, inFlightFences[crntFrame]) !=
		VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = {swapChain};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &imageIndex;

	auto result = vkQueuePresentKHR(g_device->presentQueue(), &presentInfo);

	crntFrame = (crntFrame + 1) % MAX_FRAMES_IN_FLIGHT;

	return result;
}

void SwapChain::createSwapChain() {
	SwapChainSupportDetails swapChainSupport = g_device->getSwapChainSupport();

	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount;
	if (swapChainSupport.capabilities.maxImageCount > 0 &&
		imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = g_device->surface();

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = g_device->findPhysicalQueueFamilies();
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

	createInfo.oldSwapchain = VK_NULL_HANDLE;

	VK_CHECK_RESULT(vkCreateSwapchainKHR(g_device->device(), &createInfo, nullptr, &swapChain))

	// we only specified a minimum number of images in the swap chain, so the implementation is
	// allowed to create a swap chain with more. That's why we'll first query the final number of
	// images with vkGetSwapchainImagesKHR, then resize the container and finally call it again to
	// retrieve the handles.
	vkGetSwapchainImagesKHR(g_device->device(), swapChain, &imageCount, nullptr);
	image.frameCount = imageCount;
	image.images.resize(image.frameCount);
	vkGetSwapchainImagesKHR(g_device->device(), swapChain, &imageCount, image.images.data());
	image.format = surfaceFormat.format;
	image.crntLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	image.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	image.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

	swapChainExtent = extent;
	
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

void SwapChain::createImageViews() {
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
	framebuffer.depthAttachment = {};
	framebuffer.addColorAttachment({&image, &imageView, 0});
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

		if (vkCreateImageView(g_device->device(), &viewInfo, nullptr, &swapChainImageViews[i]) !=
			VK_SUCCESS) {
		throw std::runtime_error("failed to create texture image view!");
		}
	}
	*/
}

void SwapChain::createRenderPass() {
	/*
	VkAttachmentDescription depthAttachment{};
	depthAttachment.format = findDepthFormat();
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef{};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	*/

	/*
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = getSwapChainImageFormat();
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = nullptr;//&depthAttachmentRef;

	std::array<VkSubpassDependency, 2> dependencies{};
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

	//std::array<VkAttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};
	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;//static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = &colorAttachment;//attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = dependencies.size();
	renderPassInfo.pDependencies = dependencies.data();

	if (vkCreateRenderPass(g_device->device(), &renderPassInfo, nullptr, &renderPass.renderPass) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass!");
	}
	*/

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

	for (auto& dependency : dependencies)
		renderPass.dependencies.push_back(dependency);

	renderPass.init(framebuffer.getAttachmentDescriptions());
}

void SwapChain::createFramebuffers() {
  //swapChainFramebuffers.resize(imageCount());
  /*
  framebuffer.framebuffers.resize(imageCount());
  for (size_t i = 0; i < imageCount(); i++) {
    //std::array<VkImageView, 2> attachments = {swapChainImageViews[i], depthImageViews[i]};

    VkExtent2D swapChainExtent = getSwapChainExtent();
    VkFramebufferCreateInfo framebufferInfo = {};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = renderPass.renderPass;
    framebufferInfo.attachmentCount = 1;//static_cast<uint32_t>(attachments.size());
    framebufferInfo.pAttachments = &imageViews[i].imageViews[0];//attachments.data();
    framebufferInfo.width = swapChainExtent.width;
    framebufferInfo.height = swapChainExtent.height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(
            g_device->device(),
            &framebufferInfo,
            nullptr,
            &framebuffer.framebuffers[i]) != VK_SUCCESS) {
      throw std::runtime_error("failed to create framebuffer!");
    }
  }
  */
  framebuffer.frameCount = imageCount();
  framebuffer.init(&renderPass, swapChainExtent.width, swapChainExtent.height);
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

    g_device->createImageWithInfo(
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

    if (vkCreateImageView(g_device->device(), &viewInfo, nullptr, &depthImageViews[i]) != VK_SUCCESS) {
      throw std::runtime_error("failed to create texture image view!");
    }
  }
}
*/

void SwapChain::createSyncObjects() {
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	imagesInFlight.resize(imageCount(), VK_NULL_HANDLE);

	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		VK_CHECK_RESULT(vkCreateSemaphore(g_device->device(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]))
		VK_CHECK_RESULT(vkCreateSemaphore(g_device->device(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]))
		VK_CHECK_RESULT(vkCreateFence(g_device->device(), &fenceInfo, nullptr, &inFlightFences[i]))
	}
}

VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) {
  for (const auto &availableFormat : availableFormats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
        availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return availableFormat;
    }
  }

  return availableFormats[0];
}

VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) {
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

  std::cout << "V-Sync: " << (vsyncEnabled ? "true" : "false") << std::endl;
  return vsyncEnabled ? VK_PRESENT_MODE_FIFO_KHR : VK_PRESENT_MODE_IMMEDIATE_KHR;
}

VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) {
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

VkFormat SwapChain::findDepthFormat() {
    return g_device->findSupportedFormat(
      {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
      VK_IMAGE_TILING_OPTIMAL,
      VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

void SwapChain::createCommandBuffers(std::vector<VkCommandBuffer>& commandBuffers) {
  commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = g_device->getCommandPool();
  allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

  VK_CHECK_RESULT(vkAllocateCommandBuffers(g_device->device(), &allocInfo, commandBuffers.data()))
}

} //namespace lv
