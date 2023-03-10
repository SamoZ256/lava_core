#ifndef LV_VULKAN_DESCRIPTOR_SET_H
#define LV_VULKAN_DESCRIPTOR_SET_H

#include <memory>
#include <unordered_map>
#include <vector>
#include <cassert>
#include <stdexcept>
#include <map>
#include <iostream>

#include "buffer.hpp"
#include "sampler.hpp"
#include "pipeline_layout.hpp"

namespace lv {
 
class Vulkan_DescriptorWriter {
public:
    Vulkan_PipelineLayout& pipelineLayout;
    uint8_t layoutIndex;
    std::vector<VkWriteDescriptorSet> writes;

    Vulkan_DescriptorWriter(Vulkan_PipelineLayout& aPipelineLayout, uint8_t aLayoutIndex) : pipelineLayout(aPipelineLayout), layoutIndex(aLayoutIndex) {}
    
    void writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
    void writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);
    
    void build(VkDescriptorSet &set);
    void overwrite(VkDescriptorSet &set);
};

class Vulkan_DescriptorSet {
public:
    uint8_t frameCount = 0;

    std::vector<VkDescriptorSet> descriptorSets;

    //uint16_t shaderType;
    Vulkan_PipelineLayout* pipelineLayout;
    uint8_t layoutIndex;
    VkDescriptorPool* pool;

    std::vector<Vulkan_BufferInfo> bufferInfos;
    std::vector<uint32_t> bufferBindingIndices;
    std::vector<Vulkan_ImageInfo> imageInfos;
    std::vector<uint32_t> imageBindingIndices;
    std::vector<VkDescriptorType> descriptorTypes;

    void init();

    void destroy();

    void addBinding(Vulkan_BufferInfo bufferInfo, uint32_t binding);

    void addBinding(Vulkan_ImageInfo imageInfo, uint32_t binding);

    void bind();

    static bool registerDescriptor(VkDescriptorType descriptorType);

    void registerDescriptorSet();
};

struct Vulkan_DescriptorPoolCreateInfo {
    uint32_t maxSets = 1024;
	std::map<VkDescriptorType, uint16_t> poolSizes = {
		{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 4},
    	{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4}
	};
};

class Vulkan_DescriptorPool {
public:
    VkDescriptorPool descriptorPool;

    std::vector<VkDescriptorPool> oldPools;

    std::vector<VkDescriptorPoolSize> poolSizesVec{};
    uint32_t maxSets = 1024;
    VkDescriptorPoolCreateFlags poolFlags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

    std::map<VkDescriptorType, uint16_t> poolSizesBegin;
    std::map<VkDescriptorType, uint16_t> poolSizes;

    Vulkan_DescriptorPool(Vulkan_DescriptorPoolCreateInfo& createInfo);

    void init();

    void destroy();

    //void addPoolSize(VkDescriptorType descriptorType, uint32_t count);
    
    void allocateDescriptorSet(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;
    
    //void freeDescriptorSets(std::vector<VkDescriptorSet> &descriptors) const;
    
    //void resetPool();

    void recreate();
};

extern Vulkan_DescriptorPool* g_vulkan_descriptorPool;

} //namespace lv

#endif
