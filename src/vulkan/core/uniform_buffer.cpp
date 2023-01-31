#include "vulkan/lvcore/core/uniform_buffer.hpp"

#include <vector>

#include "vulkan/lvcore/core/common.hpp"

#include "vulkan/lvcore/core/buffer_helper.hpp"
#include "vulkan/lvcore/core/allocator.hpp"
#include "vulkan/lvcore/core/swap_chain.hpp"

namespace lv {

Vulkan_UniformBuffer::Vulkan_UniformBuffer(size_t aSize) : size(aSize) {
	if (frameCount == 0) frameCount = g_vulkan_swapChain->maxFramesInFlight;

	buffers.resize(frameCount);
	allocations.resize(frameCount);
	//allocInfos.resize(MAX_FRAMES_IN_FLIGHT);

	for (uint8_t i = 0; i < frameCount; i++) {
		//VmaAllocationInfo allocInfo;
		allocations[i] = Vulkan_BufferHelper::createBuffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, buffers[i], nullptr, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		//std::cout << "Offset: " << allocInfo.offset << std::endl;
		//std::cout << "Size: " << allocInfo.size << std::endl;
	}
}

void Vulkan_UniformBuffer::destroy() {
	for (uint8_t i = 0; i < frameCount; i++) {
		vmaDestroyBuffer(g_vulkan_allocator->allocator, buffers[i], allocations[i]);
	}
}

/*
VkDescriptorSetLayoutBinding UniformBuffer::getBinding(uint8_t binding) {
  VkDescriptorSetLayoutBinding layoutBinding{};
  layoutBinding.binding = binding;
  layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  layoutBinding.descriptorCount = 1;
  layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
  layoutBinding.pImmutableSamplers = nullptr;

  return layoutBinding;
}
*/

Vulkan_BufferInfo Vulkan_UniformBuffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset) {
	Vulkan_BufferInfo info;
	info.infos.resize(frameCount);
	for (uint8_t i = 0; i < frameCount; i++) {
		info.infos[i].buffer = buffers[i];// = VkDescriptorBufferInfo{buffers[i], offset, size};
		info.infos[i].offset = offset;
		info.infos[i].range = size;
	}
	info.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

	return info;
}

void Vulkan_UniformBuffer::upload(void* uploadData) {
	void* data;
	vmaMapMemory(g_vulkan_allocator->allocator, allocations[g_vulkan_swapChain->imageIndex], &data);
	memcpy(data, uploadData, size);
	vmaUnmapMemory(g_vulkan_allocator->allocator, allocations[g_vulkan_swapChain->imageIndex]);
	//memcpy(allocInfos[Swap::g_metal_swapChain.imageIndex].pMappedData, uploadData, size);
}

} //namespace lv
