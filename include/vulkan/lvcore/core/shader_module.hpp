#ifndef LV_VULKAN_SHADER_MODULE_H
#define LV_VULKAN_SHADER_MODULE_H

#include "swap_chain.hpp"
#include "shader_bundle.hpp"

namespace lv {

struct Vulkan_ShaderModuleCreateInfo {
    Vulkan_ShaderBundle* shaderBundle;
    VkShaderStageFlagBits shaderStage;
    std::string source;
    std::vector<VkSpecializationMapEntry> specializationConstants;
    void* constantsData = nullptr;
    size_t constantsSize;
};

class Vulkan_ShaderModule {
public:
    VkShaderModule ID;
    VkPipelineShaderStageCreateInfo stageInfo;
    VkSpecializationInfo specializationInfo{};

    Vulkan_ShaderModuleCreateInfo createInfo;
    Vulkan_ShaderBundle* shaderBundle;

    void init(Vulkan_ShaderModuleCreateInfo& aCreateInfo);

    void destroy();

    void compile();

    void recompile();
};

} //namespace lv

#endif
