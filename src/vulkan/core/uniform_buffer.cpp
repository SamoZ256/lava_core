#include "lvcore/core/uniform_buffer.hpp"

#include <vector>

#include "lvcore/core/common.hpp"

#include "lvcore/core/buffer_helper.hpp"
#include "lvcore/core/allocator.hpp"
#include "lvcore/core/swap_chain.hpp"

namespace lv {

UniformBuffer::UniformBuffer(size_t aSize) : size(aSize) {
	buffers.resize(MAX_FRAMES_IN_FLIGHT);
	allocations.resize(MAX_FRAMES_IN_FLIGHT);
	//allocInfos.resize(MAX_FRAMES_IN_FLIGHT);

	for (uint8_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		//VmaAllocationInfo allocInfo;
		allocations[i] = BufferHelper::createBuffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, buffers[i], nullptr, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		//std::cout << "Offset: " << allocInfo.offset << std::endl;
		//std::cout << "Size: " << allocInfo.size << std::endl;
	}
}

void UniformBuffer::destroy() {
	for (uint8_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
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
	BufferInfo bufferInfo;
	bufferInfo.infos.resize(MAX_FRAMES_IN_FLIGHT);
	for (uint8_t i = 0; i < bufferInfo.infos.size(); i++) {
		bufferInfo.infos[i] = VkDescriptorBufferInfo{buffers[i], offset, size};
	}

	return bufferInfo;
}

void UniformBuffer::upload(void* uploadData) {
	void* data;
	vmaMapMemory(g_allocator->allocator, allocations[g_swapChain->imageIndex], &data);
	memcpy(data, uploadData, size);
	vmaUnmapMemory(g_allocator->allocator, allocations[g_swapChain->imageIndex]);
	//memcpy(allocInfos[Swap::g_swapChain.imageIndex].pMappedData, uploadData, size);
}

} //namespace lv
