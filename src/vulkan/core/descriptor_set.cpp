#include "lvcore/core/descriptor_set.hpp"

#include "lvcore/core/common.hpp"

#include "lvcore/core/device.hpp"
#include "lvcore/core/swap_chain.hpp"
//#include "Core/Renderer.hpp"

namespace lv {

DescriptorPool* g_descriptorPool = nullptr;

// *************** Descriptor Set Layout *********************

void DescriptorSetLayout::init() {
	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
	descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	descriptorSetLayoutInfo.pBindings = bindings.data();

	VK_CHECK_RESULT(vkCreateDescriptorSetLayout(g_device->device(), &descriptorSetLayoutInfo, nullptr, &descriptorSetLayout))
}

void DescriptorSetLayout::destroy() {
	vkDestroyDescriptorSetLayout(g_device->device(), descriptorSetLayout, nullptr);
	//descriptorSetLayout = VK_NULL_HANDLE;
}

void DescriptorSetLayout::addBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags) {
	//assert(bindings.count(binding) == 0 && "Binding already in use");
	VkDescriptorSetLayoutBinding layoutBinding{};
	layoutBinding.binding = binding;
	layoutBinding.descriptorType = descriptorType;
	layoutBinding.descriptorCount = 1;
	layoutBinding.stageFlags = stageFlags;
	bindings.push_back(layoutBinding);
}

// *************** Descriptor Pool *********************

DescriptorPool::DescriptorPool(DescriptorPoolCreateInfo& createInfo) {
	poolSizesBegin = createInfo.poolSizes;
	poolSizes = poolSizesBegin;

	for (const auto& [key, value] : poolSizes) {
		poolSizesVec.push_back({key, (unsigned)value * MAX_FRAMES_IN_FLIGHT});
	}

	init();

	g_descriptorPool = this;
}

void DescriptorPool::init() {
	VkDescriptorPoolCreateInfo descriptorPoolInfo{};
	descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	descriptorPoolInfo.pPoolSizes = poolSizesVec.data();
	descriptorPoolInfo.maxSets = maxSets;
	descriptorPoolInfo.flags = poolFlags;

	VK_CHECK_RESULT(vkCreateDescriptorPool(g_device->device(), &descriptorPoolInfo, nullptr, &descriptorPool))
}

void DescriptorPool::destroy() {
  	vkDestroyDescriptorPool(g_device->device(), descriptorPool, nullptr);
}

void DescriptorPool::allocateDescriptorSet(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const {
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.pSetLayouts = &descriptorSetLayout;
	allocInfo.descriptorSetCount = 1;

	// Might want to create a "DescriptorPoolManager" class that handles this case, and builds
	// a new pool whenever an old pool fills up. But this is beyond our current scope
	VK_CHECK_RESULT(vkAllocateDescriptorSets(g_device->device(), &allocInfo, &descriptor))
}

/*
void DescriptorPool::freeDescriptorSets(std::vector<VkDescriptorSet> &descriptors) const {
  vkFreeDescriptorSets(
      g_device->device(),
      descriptorPool,
      static_cast<uint32_t>(descriptors.size()),
      descriptors.data());
}
*/

/*
void DescriptorPool::resetPool() {
  vkResetDescriptorPool(g_device->device(), descriptorPool, 0);
}
*/

/*
void DescriptorPool::addPoolSize(VkDescriptorType descriptorType, uint32_t count) {
  poolSizes.push_back({descriptorType, count});
}
*/

void DescriptorPool::recreate() {
	oldPools.push_back(descriptorPool);
	//descriptorPool = VK_NULL_HANDLE;
	init();
	poolSizes = poolSizesBegin;
}

// *************** Descriptor Writer *********************

void DescriptorWriter::writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo) {
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

void DescriptorWriter::writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo) {
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

void DescriptorWriter::build(VkDescriptorSet &set) {
	g_descriptorPool->allocateDescriptorSet(pipelineLayout.descriptorSetLayouts[layoutIndex].descriptorSetLayout, set);
	overwrite(set);
}

void DescriptorWriter::overwrite(VkDescriptorSet &set) {
	for (auto &write : writes) {
		write.dstSet = set;
	}
	//std::cout << "Writes: " << writes.size() << std::endl;
	vkUpdateDescriptorSets(g_device->device(), writes.size(), writes.data(), 0, nullptr);
}

// *************** Descriptor Set *********************

/*
void DescriptorSet::destroy() {
  g_descriptorManager->descriptorPool.freeDescriptorSets(descriptorSets);
}
*/

void DescriptorSet::init() {
	registerDescriptorSet();
	
	descriptorSets.resize(frameCount);
	for (uint8_t i = 0; i < descriptorSets.size(); i++) {
		DescriptorWriter writer(pipelineLayout, layoutIndex);
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
	pool = &g_descriptorPool->descriptorPool;
	//std::cout << "Setting pool end" << std::endl;
}

void DescriptorSet::destroy() {
	//g_descriptorManager->descriptorPool.freeDescriptorSets(descriptorSets);
	vkFreeDescriptorSets(g_device->device(), *pool, descriptorSets.size(), descriptorSets.data());
	bufferInfos.clear();
	bufferBindingIndices.clear();
	imageInfos.clear();
	imageBindingIndices.clear();
	descriptorTypes.clear();
}

void DescriptorSet::addBufferBinding(BufferInfo bufferInfo, uint32_t binding, VkDescriptorType descriptorType) {
    //g_descriptorManager->descriptorPool.addPoolSize(descriptorType, SwapChain::MAX_FRAMES_IN_FLIGHT);
	for (uint8_t i = 0; i < frameCount; i++)
    	descriptorTypes.push_back(descriptorType);
    bufferBindingIndices.push_back(binding);
    bufferInfos.push_back(bufferInfo);
}

void DescriptorSet::addImageBinding(ImageInfo imageInfo, uint32_t binding, VkDescriptorType descriptorType) {
    //g_descriptorManager->descriptorPool.addPoolSize(descriptorType, SwapChain::MAX_FRAMES_IN_FLIGHT);
	for (uint8_t i = 0; i < frameCount; i++)
    	descriptorTypes.push_back(descriptorType);
    imageBindingIndices.push_back(binding);
    imageInfos.push_back(imageInfo);
}

void DescriptorSet::bind() {
  vkCmdBindDescriptorSets(g_swapChain->getActiveCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout.pipelineLayout, layoutIndex, 1, &descriptorSets[g_swapChain->imageIndex], 0, nullptr);
}

bool DescriptorSet::registerDescriptor(VkDescriptorType descriptorType) {
	uint16_t& count = g_descriptorPool->poolSizes[descriptorType];
	if (count == 0) {
		g_descriptorPool->recreate();
		count = g_descriptorPool->poolSizes[descriptorType];
		count -= 1;
		return false;
	}
	count -= 1;
	//std::cout << descriptorType << " : " << count << std::endl;

	return true;
}

void DescriptorSet::registerDescriptorSet() {
	while (true) {
		bool allocated = true;
		for (auto& descriptorType : descriptorTypes) {
			if (!registerDescriptor(descriptorType))
				allocated = false;
		}
		if (allocated)
			break;
	}
}

// *************** Descriptor Layouts *********************

void PipelineLayout::init() {
	for (uint8_t i = 0; i < descriptorSetLayouts.size(); i++)
		descriptorSetLayouts[i].init();

	std::vector<VkDescriptorSetLayout> descLayouts;
	for (uint8_t i = 0; i < descriptorSetLayouts.size(); i++) {
		descLayouts.push_back(descriptorSetLayouts[i].descriptorSetLayout);
	} 

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = descLayouts.size();
	pipelineLayoutInfo.pSetLayouts = descLayouts.data();
	pipelineLayoutInfo.pushConstantRangeCount = pushConstantRanges.size();
	pipelineLayoutInfo.pPushConstantRanges = pushConstantRanges.data();

	VK_CHECK_RESULT(vkCreatePipelineLayout(g_device->device(), &pipelineLayoutInfo, nullptr, &pipelineLayout))
}

void PipelineLayout::destroy() {
	for (uint8_t i = 0; i < descriptorSetLayouts.size(); i++)
		descriptorSetLayouts[i].destroy();
	vkDestroyPipelineLayout(g_device->device(), pipelineLayout, nullptr);
	//std::cout << "PIPELINE LAYOUT DESTROYED" << std::endl;
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
