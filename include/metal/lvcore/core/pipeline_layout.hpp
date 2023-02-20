#ifndef LV_METAL_PIPELINE_LAYOUT_H
#define LV_METAL_PIPELINE_LAYOUT_H

#include <vector>

#include "enums.hpp"

namespace lv {

struct Metal_DescriptorSetLayoutBinding {
    LvDescriptorType descriptorType;
    LvShaderStage shaderStage;
};

struct Metal_PushConstantRange {
    LvShaderStage stageFlags;
    uint32_t offset;
    uint32_t size;
};

class Metal_DescriptorSetLayout {
public:
    std::vector<Metal_DescriptorSetLayoutBinding> bindings;

    Metal_DescriptorSetLayout() = default;

    void init() {}
    
    void destroy() {}

    //DescriptorSetLayout(const DescriptorSetLayout&) = delete;
    //Vulkan_DescriptorSetLayout& operator=(const Vulkan_DescriptorSetLayout&) = delete;

    void addBinding(uint32_t binding, LvDescriptorType descriptorType, LvShaderStage shaderStage) {
        bindings.push_back({descriptorType, shaderStage});
    }
};

class Metal_PipelineLayout {
public:
    std::vector<Metal_DescriptorSetLayout> descriptorSetLayouts;
    std::vector<Metal_PushConstantRange> pushConstantRanges;

    void init() {}

    void destroy() {}
};

} //namespace lv

#endif
