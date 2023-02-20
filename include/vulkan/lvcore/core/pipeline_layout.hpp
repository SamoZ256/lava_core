#ifndef LV_VULKAN_PIPELINE_LAYOUT_H
#define LV_VULKAN_PIPELINE_LAYOUT_H

#include <vector>

#include <vulkan/vulkan.h>

namespace lv {

class Vulkan_DescriptorSetLayout {
public:
    VkDescriptorSetLayout descriptorSetLayout;
    std::vector<VkDescriptorSetLayoutBinding> bindings;

    Vulkan_DescriptorSetLayout() = default;

    void init();
    
    void destroy();

    //DescriptorSetLayout(const DescriptorSetLayout&) = delete;
    //Vulkan_DescriptorSetLayout& operator=(const Vulkan_DescriptorSetLayout&) = delete;

    void addBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags shaderStage);
};

class Vulkan_PipelineLayout {
public:
    std::vector<Vulkan_DescriptorSetLayout> descriptorSetLayouts;
    VkPipelineLayout pipelineLayout;
    std::vector<VkPushConstantRange> pushConstantRanges;

    void init();

    void destroy();
};

} //namespace lv

#endif
