#ifndef LV_OPENGL_PIPELINE_LAYOUT_H
#define LV_OPENGL_PIPELINE_LAYOUT_H

#include <vector>

#include "enums.hpp"

namespace lv {

struct OpenGL_DescriptorSetLayoutBinding {
    LvDescriptorType descriptorType;
    LvShaderStageFlags shaderStage;
};

struct OpenGL_PushConstantRange {
    LvShaderStageFlags stageFlags;
    uint32_t offset;
    uint32_t size;
};

class OpenGL_DescriptorSetLayout {
public:
    std::vector<OpenGL_DescriptorSetLayoutBinding> bindings;

    //OpenGL_DescriptorSetLayout() = default;

    void init() {}
    
    void destroy() {}

    //DescriptorSetLayout(const DescriptorSetLayout&) = delete;
    //Vulkan_DescriptorSetLayout& operator=(const Vulkan_DescriptorSetLayout&) = delete;

    void addBinding(uint32_t binding, LvDescriptorType descriptorType, LvShaderStageFlags shaderStage) {
        bindings.push_back({descriptorType, shaderStage});
    }
};

class OpenGL_PipelineLayout {
public:
    std::vector<OpenGL_DescriptorSetLayout> descriptorSetLayouts;
    std::vector<OpenGL_PushConstantRange> pushConstantRanges;

    void init() {}

    void destroy() {}
};

} //namespace lv

#endif
