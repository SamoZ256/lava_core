#include "vulkan/lvcore/core/descriptor_set.hpp"

#include "vulkan/lvcore/core/common.hpp"

#include "vulkan/lvcore/core/device.hpp"
#include "vulkan/lvcore/core/swap_chain.hpp"
//#include "Core/Renderer.hpp"

namespace lv {

Vulkan_DescriptorPool* g_vulkan_descriptorPool = nullptr;

// *************** Descriptor Pool *********************

Vulkan_DescriptorPool::Vulkan_DescriptorPool(Vulkan_DescriptorPoolCreateInfo& createInfo) {
	maxSets = createInfo.maxSets;
	poolSizesBegin = createInfo.poolSizes;
	poolSizes = poolSizesBegin;

	for (const auto& [key, value] : poolSizes) {
		poolSizesVec.push_back({key, (unsigned)value * g_vulkan_swapChain->maxFramesInFlight});
	}

	init();

	g_vulkan_descriptorPool = this;
}

void Vulkan_DescriptorPool::init() {
	VkDescriptorPoolCreateInfo descriptorPoolInfo{};
	descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizesVec.size());
	descriptorPoolInfo.pPoolSizes = poolSizesVec.data();
	descriptorPoolInfo.maxSets = maxSets;
	descriptorPoolInfo.flags = poolFlags;

	VK_CHECK_RESULT(vkCreateDescriptorPool(g_vulkan_device->device(), &descriptorPoolInfo, nullptr, &descriptorPool));
}

void Vulkan_DescriptorPool::destroy() {
  	vkDestroyDescriptorPool(g_vulkan_device->device(), descriptorPool, nullptr);
}

void Vulkan_DescriptorPool::allocateDescriptorSet(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const {
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.pSetLayouts = &descriptorSetLayout;
	allocInfo.descriptorSetCount = 1;

	// Might want to create a "DescriptorPoolManager" class that handles this case, and builds
	// a new pool whenever an old pool fills up. But this is beyond our current scope
	VK_CHECK_RESULT(vkAllocateDescriptorSets(g_vulkan_device->device(), &allocInfo, &descriptor));
}

/*
void DescriptorPool::freeDescriptorSets(std::vector<VkDescriptorSet> &descriptors) const {
  vkFreeDescriptorSets(
      g_metal_device->device(),
      descriptorPool,
      static_cast<uint32_t>(descriptors.size()),
      descriptors.data());
}
*/

/*
void DescriptorPool::resetPool() {
  vkResetDescriptorPool(g_metal_device->device(), descriptorPool, 0);
}
*/

/*
void DescriptorPool::addPoolSize(VkDescriptorType descriptorType, uint32_t count) {
  poolSizes.push_back({descriptorType, count});
}
*/

void Vulkan_DescriptorPool::recreate() {
	std::cout << "Recreating descriptor pool" << std::endl;
	oldPools.push_back(descriptorPool);
	//descriptorPool = VK_NULL_HANDLE;
	poolSizes = poolSizesBegin;
	init();
}

// *************** Descriptor Writer *********************

void Vulkan_DescriptorWriter::writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo) {
	//assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

	auto &bindingDescription = pipelineLayout.descriptorSetLayouts[layoutIndex].bindings[binding];

	assert(bindingDescription.descriptorCount == 1 && "Binding single descriptor info, but binding expects multiple");

	VkWriteDescriptorSet write{};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.descriptorType = bindingDescription.descriptorType;
	write.dstBinding = binding;
	write.pBufferInfo = bufferInfo;
	write.descriptorCount = 1;

	writes.push_back(write);
}

void Vulkan_DescriptorWriter::writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo) {
	//assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

	auto &bindingDescription = pipelineLayout.descriptorSetLayouts[layoutIndex].bindings[binding];

	assert(bindingDescription.descriptorCount == 1 && "Binding single descriptor info, but binding expects multiple");

	VkWriteDescriptorSet write{};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.descriptorType = bindingDescription.descriptorType;
	write.dstBinding = binding;
	write.pImageInfo = imageInfo;
	write.descriptorCount = 1;

	writes.push_back(write);
}

void Vulkan_DescriptorWriter::build(VkDescriptorSet &set) {
	g_vulkan_descriptorPool->allocateDescriptorSet(pipelineLayout.descriptorSetLayouts[layoutIndex].descriptorSetLayout, set);
	overwrite(set);
}

void Vulkan_DescriptorWriter::overwrite(VkDescriptorSet &set) {
	for (auto &write : writes) {
		write.dstSet = set;
	}
	//std::cout << "Writes: " << writes.size() << std::endl;
	vkUpdateDescriptorSets(g_vulkan_device->device(), writes.size(), writes.data(), 0, nullptr);
}

// *************** Descriptor Set *********************

/*
void DescriptorSet::destroy() {
  g_descriptorManager->descriptorPool.freeDescriptorSets(descriptorSets);
}
*/

void Vulkan_DescriptorSet::init() {
	if (frameCount == 0) frameCount = g_vulkan_swapChain->maxFramesInFlight;

	registerDescriptorSet();
	
	descriptorSets.resize(frameCount);
	for (uint8_t i = 0; i < descriptorSets.size(); i++) {
		Vulkan_DescriptorWriter writer(pipelineLayout, layoutIndex);
		for (uint8_t buff = 0; buff < bufferInfos.size(); buff++) {
			uint8_t frameIndex = i < bufferInfos[buff].infos.size() ? i : 0;
			writer.writeBuffer(bufferBindingIndices[buff], &bufferInfos[buff].infos[frameIndex]);
		}
		for (uint8_t img = 0; img < imageInfos.size(); img++) {
			uint8_t frameIndex = i < imageInfos[img].infos.size() ? i : 0;
			writer.writeImage(imageBindingIndices[img], &imageInfos[img].infos[frameIndex]);
		}
		//std::cout << "Descriptor set: " << (int)i << std::endl;
		writer.build(descriptorSets[i]);
	}

	//std::cout << "Setting pool start" << std::endl;
	pool = &g_vulkan_descriptorPool->descriptorPool;
	//std::cout << "Setting pool end" << std::endl;
}

void Vulkan_DescriptorSet::destroy() {
	//g_descriptorManager->descriptorPool.freeDescriptorSets(descriptorSets);
	vkFreeDescriptorSets(g_vulkan_device->device(), *pool, descriptorSets.size(), descriptorSets.data());
	bufferInfos.clear();
	bufferBindingIndices.clear();
	imageInfos.clear();
	imageBindingIndices.clear();
	descriptorTypes.clear();
}

void Vulkan_DescriptorSet::addBinding(Vulkan_BufferInfo bufferInfo, uint32_t binding) {
    //g_descriptorManager->descriptorPool.addPoolSize(descriptorType, SwapChain::MAX_FRAMES_IN_FLIGHT);
    descriptorTypes.push_back(bufferInfo.descriptorType);
    bufferBindingIndices.push_back(binding);
    bufferInfos.push_back(bufferInfo);
}

void Vulkan_DescriptorSet::addBinding(Vulkan_ImageInfo imageInfo, uint32_t binding) {
    //g_descriptorManager->descriptorPool.addPoolSize(descriptorType, SwapChain::MAX_FRAMES_IN_FLIGHT);
    descriptorTypes.push_back(imageInfo.descriptorType);
    imageBindingIndices.push_back(binding);
    imageInfos.push_back(imageInfo);
}

void Vulkan_DescriptorSet::bind() {
  	vkCmdBindDescriptorSets(g_vulkan_swapChain->getActiveCommandBuffer(), g_vulkan_swapChain->pipelineBindPoint, pipelineLayout.pipelineLayout, layoutIndex, 1, &descriptorSets[g_vulkan_swapChain->imageIndex], 0, nullptr);
}

bool Vulkan_DescriptorSet::registerDescriptor(VkDescriptorType descriptorType) {
	uint16_t& count = g_vulkan_descriptorPool->poolSizes[descriptorType];
	if (count == 0) {
		g_vulkan_descriptorPool->recreate();
		count = g_vulkan_descriptorPool->poolSizes[descriptorType];
		count -= 1;

		return false;
	}
	count -= 1;
	//std::cout << descriptorType << " : " << count << std::endl;

	return true;
}

void Vulkan_DescriptorSet::registerDescriptorSet() {
	while (true) {
		bool allocated = true;
		for (uint8_t i = 0; i < frameCount; i++) {
			for (auto& descriptorType : descriptorTypes) {
				if (!registerDescriptor(descriptorType))
					allocated = false;
			}
		}
		if (allocated)
			break;
	}
}

// *************** Descriptor Manager *********************

/*
DescriptorManager::DescriptorManager(DescriptorManagerCreateInfo& createInfo) {
	//Pool sizes
	poolSizesBegin = createInfo.poolSizes;
	poolSizes = poolSizesBegin;
	for (const auto& [key, value] : poolSizes) {
		descriptorPool.addPoolSize(key, value * MAX_FRAMES_IN_FLIGHT);
	}
	descriptorPool.init();
	//pipelineLayouts.resize(createInfo.pipelineLayoutCount);

	g_descriptorManager = this;
}
*/

/*
void DescriptorManager::init(uint16_t pipelineLayoutCount) {
  descriptorPool.init();
  shaderLayouts.resize(pipelineLayoutCount);
}
*/

/*
void DescriptorManager::destroy() {
	std::cout << "Destroying decriptor manager" << std::endl;
	for (uint8_t layout = 0; layout < pipelineLayouts.size(); layout++) {
		pipelineLayouts[layout].destroy();
	}

	descriptorPool.destroy();
	for (uint8_t i = 0; i < shadowGraphicsPipelineLayouts.layouts.size(); i++) {
		shadowGraphicsPipelineLayouts.layouts[i].destroy();
	}
	std::cout << "Destroyed decriptor manager" << std::endl;
}

void DescriptorManager::resetPool() {
  	std::cout << "Resetting descriptor pool" << std::endl;
	descriptorPool.recreate();
	poolSizes = poolSizesBegin;
}
*/

/*
void DescriptorManager::createDescriptorLayouts() {
	for (uint8_t layout = 0; layout < pipelineLayouts.size(); layout++) {
		pipelineLayouts[layout].init();
	}
	//for (uint8_t layout = 0; layout < shaderLayouts.size(); layout++)
	//  shaderLayouts[layout].createPipelineLayout();
}
*/

} //namespace lv
