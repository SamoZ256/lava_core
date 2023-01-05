#ifndef LV_DESCRIPTOR_SET_H
#define LV_DESCRIPTOR_SET_H

#include <memory>
#include <unordered_map>
#include <vector>
#include <cassert>
#include <stdexcept>
#include <map>
#include <iostream>

#include "uniform_buffer.hpp"
#include "sampler.hpp"

//Macros
//#define GET_SHADER_LAYOUT(shaderType) g_descriptorManager->shaderLayouts[shaderType]
//#define GET_DESCRIPTOR_SET_LAYOUT(shaderType, layoutIndex) g_descriptorManager->shaderLayouts[shaderType].layouts[layoutIndex]

namespace lv {

class DescriptorSet;

class DescriptorSetLayout {
 public:
  DescriptorSetLayout() = default;
  
  void destroy();

  //DescriptorSetLayout(const DescriptorSetLayout&) = delete;
  DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;

  void addBinding(
        uint32_t binding,
        VkDescriptorType descriptorType,
        VkShaderStageFlags stageFlags);
 
  VkDescriptorSetLayout descriptorSetLayout;
  std::vector<VkDescriptorSetLayoutBinding> bindings;

  void init();
};

class PipelineLayout {
public:
  std::vector<DescriptorSetLayout> descriptorSetLayouts;
  VkPipelineLayout pipelineLayout;
  std::vector<VkPushConstantRange> pushConstantRanges;

  void init();

  void destroy();
};
 
class DescriptorWriter {
 public:
  //uint8_t layouts;
  PipelineLayout& pipelineLayout;
  uint8_t layoutIndex;
  std::vector<VkWriteDescriptorSet> writes;

  DescriptorWriter(PipelineLayout& aPipelineLayout, uint8_t aLayoutIndex) : pipelineLayout(aPipelineLayout), layoutIndex(aLayoutIndex) {}
 
  void writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
  void writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);
 
  void build(VkDescriptorSet &set);
  void overwrite(VkDescriptorSet &set);
};

class DescriptorSet {
public:
    uint8_t frameCount = MAX_FRAMES_IN_FLIGHT;

    std::vector<VkDescriptorSet> descriptorSets;

    //uint16_t shaderType;
    PipelineLayout& pipelineLayout;
    uint8_t layoutIndex;
    VkDescriptorPool* pool;

    std::vector<BufferInfo> bufferInfos;
    std::vector<uint32_t> bufferBindingIndices;
    std::vector<ImageInfo> imageInfos;
    std::vector<uint32_t> imageBindingIndices;
    std::vector<VkDescriptorType> descriptorTypes;

    DescriptorSet(PipelineLayout& aPipelineLayout, uint8_t aLayoutIndex) : pipelineLayout(aPipelineLayout), layoutIndex(aLayoutIndex) {}

    //void destroy();

    void init();

    void destroy();

    void addBufferBinding(BufferInfo bufferInfo, uint32_t binding, VkDescriptorType descriptorType);

    void addImageBinding(ImageInfo imageInfo, uint32_t binding, VkDescriptorType descriptorType);

    void bind();

    static bool registerDescriptor(VkDescriptorType descriptorType);

    void registerDescriptorSet();
};

struct DescriptorPoolCreateInfo {
	//uint16_t pipelineLayoutCount;
	std::map<VkDescriptorType, uint16_t> poolSizes = {
		{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 4},
    	{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4}
	};
};

class DescriptorPool {
 public:
  VkDescriptorPool descriptorPool;

  std::vector<VkDescriptorPool> oldPools;

  std::vector<VkDescriptorPoolSize> poolSizesVec{};
  uint32_t maxSets = 1024;
  VkDescriptorPoolCreateFlags poolFlags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

	std::map<VkDescriptorType, uint16_t> poolSizesBegin;
	std::map<VkDescriptorType, uint16_t> poolSizes;

  DescriptorPool(DescriptorPoolCreateInfo& createInfo);

  void init();

  void destroy();

  //void addPoolSize(VkDescriptorType descriptorType, uint32_t count);
 
  void allocateDescriptorSet(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;
 
  //void freeDescriptorSets(std::vector<VkDescriptorSet> &descriptors) const;
 
  //void resetPool();

  void recreate();
};

/*
class DescriptorManager {
public:
	//static constexpr uint8_t SHADER_COUNT = 1;

	DescriptorPool descriptorPool;

	std::map<VkDescriptorType, uint16_t> poolSizesBegin {
		{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 64},
		{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 512}
	};

	//std::map<VkDescriptorType, uint16_t> poolSizesBegin;
	//std::map<VkDescriptorType, uint16_t> poolSizes;

	//void init(uint16_t pipelineLayoutCount);

	DescriptorManager(DescriptorManagerCreateInfo& createInfo);

	void destroy();

	void resetPool();

	//void createDescriptorLayouts();

	//PipelineLayout& getPipelineLayout(uint16_t shaderType) { return pipelineLayouts[shaderType]; }

	//DescriptorSetLayout& getDescriptorSetLayout(uint16_t shaderType, uint8_t layoutIndex) { return pipelineLayouts[shaderType].layouts[layoutIndex]; }

	//DescriptorSetLayout& getDescriptorLayout(uint16_t shaderType, uint8_t layoutIndex) { return shaderLayouts[shaderType].layouts[layoutIndex]; }

//private:
	//Layouts
	//std::vector<PipelineLayout> pipelineLayouts;
};
*/

extern DescriptorPool* g_descriptorPool;

/*
class Desc {
public:
  static DescriptorManager* g_descriptorManager;
};
*/

} //namespace lv

#endif
