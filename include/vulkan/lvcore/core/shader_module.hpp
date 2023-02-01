#ifndef LV_VULKAN_SHADER_MODULE_H
#define LV_VULKAN_SHADER_MODULE_H

#include "swap_chain.hpp"

namespace lv {

struct Vulkan_ShaderModuleCreateInfo {
    VkShaderStageFlagBits shaderType;
    std::string source;
    std::vector<VkSpecializationMapEntry> specializationConstants;
    void* constantsData = nullptr;
    uint32_t constantsSize;
};

class Vulkan_ShaderModule {
public:
    VkShaderModule ID;
    VkPipelineShaderStageCreateInfo stageInfo;
    VkSpecializationInfo specializationInfo{};

    Vulkan_ShaderModule(Vulkan_ShaderModuleCreateInfo& createInfo);

    void destroy();
};

} //namespace lv

#endif
