#include "lvcore/core/uniform_buffer.hpp"

#include <vector>

#include "lvcore/core/common.hpp"

#include "lvcore/core/buffer_helper.hpp"
#include "lvcore/core/allocator.hpp"
#include "lvcore/core/swap_chain.hpp"

namespace lv {

UniformBuffer::UniformBuffer(size_t aSize) : size(aSize) {
	if (frameCount == 0) frameCount = g_swapChain->maxFramesInFlight;

	buffers.resize(frameCount);
	allocations.resize(frameCount);
	//allocInfos.resize(MAX_FRAMES_IN_FLIGHT);

	for (uint8_t i = 0; i < frameCount; i++) {
		//VmaAllocationInfo allocInfo;
		allocations[i] = BufferHelper::createBuffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, buffers[i], nullptr, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		//std::cout << "Offset: " << allocInfo.offset << std::endl;
		//std::cout << "Size: " << allocInfo.size << std::endl;
	}
}

void UniformBuffer::destroy() {
	for (uint8_t i = 0; i < frameCount; i++) {
		vmaDestroyBuffer(g_allocator->allocator, buffers[i], allocations[i]);
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

BufferInfo UniformBuffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset) {
	BufferInfo info;
	info.infos.resize(frameCount);
	for (uint8_t i = 0; i < frameCount; i++) {
		info.infos[i].buffer = buffers[i];// = VkDescriptorBufferInfo{buffers[i], offset, size};
		info.infos[i].offset = offset;
		info.infos[i].range = size;
	}
	info.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

	return info;
}

void UniformBuffer::upload(void* uploadData) {
	void* data;
	vmaMapMemory(g_allocator->allocator, allocations[g_swapChain->imageIndex], &data);
	memcpy(data, uploadData, size);
	vmaUnmapMemory(g_allocator->allocator, allocations[g_swapChain->imageIndex]);
	//memcpy(allocInfos[Swap::g_swapChain.imageIndex].pMappedData, uploadData, size);
}

} //namespace lv
