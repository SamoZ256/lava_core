#include "vulkan/lvcore/core/pipeline_layout.hpp"

#include "vulkan/lvcore/core/common.hpp"

#include "vulkan/lvcore/core/device.hpp"

namespace lv {

// *************** Descriptor Set Layout *********************

void Vulkan_DescriptorSetLayout::init() {
	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
	descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	descriptorSetLayoutInfo.pBindings = bindings.data();

	VK_CHECK_RESULT(vkCreateDescriptorSetLayout(g_vulkan_device->device(), &descriptorSetLayoutInfo, nullptr, &descriptorSetLayout))
}

void Vulkan_DescriptorSetLayout::destroy() {
	vkDestroyDescriptorSetLayout(g_vulkan_device->device(), descriptorSetLayout, nullptr);
	//descriptorSetLayout = VK_NULL_HANDLE;
}

void Vulkan_DescriptorSetLayout::addBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags shaderStage) {
	//assert(bindings.count(binding) == 0 && "Binding already in use");
	VkDescriptorSetLayoutBinding layoutBinding{};
	layoutBinding.binding = binding;
	layoutBinding.descriptorType = descriptorType;
	layoutBinding.descriptorCount = 1;
	layoutBinding.stageFlags = shaderStage;
	bindings.push_back(layoutBinding);
}

// *************** Pipeline Layout *********************

void Vulkan_PipelineLayout::init() {
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

	VK_CHECK_RESULT(vkCreatePipelineLayout(g_vulkan_device->device(), &pipelineLayoutInfo, nullptr, &pipelineLayout));
}

void Vulkan_PipelineLayout::destroy() {
	for (uint8_t i = 0; i < descriptorSetLayouts.size(); i++)
		descriptorSetLayouts[i].destroy();
	vkDestroyPipelineLayout(g_vulkan_device->device(), pipelineLayout, nullptr);
}

} //namespace lv
