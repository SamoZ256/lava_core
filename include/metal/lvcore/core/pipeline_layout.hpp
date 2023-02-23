#ifndef LV_METAL_PIPELINE_LAYOUT_H
#define LV_METAL_PIPELINE_LAYOUT_H

#include <vector>

#include "enums.hpp"

namespace lv {

struct Metal_DescriptorSetLayoutBinding {
    LvDescriptorType descriptorType;
    LvShaderStageFlags shaderStage;
};

struct Metal_PushConstantRange {
    LvShaderStageFlags stageFlags;
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

    void addBinding(uint32_t binding, LvDescriptorType descriptorType, LvShaderStageFlags shaderStage) {
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
