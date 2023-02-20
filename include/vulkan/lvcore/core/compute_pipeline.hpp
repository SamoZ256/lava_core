#ifndef LV_VULKAN_COMPUTE_PIPELINE_H
#define LV_VULKAN_COMPUTE_PIPELINE_H

#include "shader_module.hpp"
#include "descriptor_set.hpp"

namespace lv {

struct Vulkan_ComputePipelineCreateInfo {
    Vulkan_ShaderModule* computeShaderModule;
    Vulkan_PipelineLayout* pipelineLayout;
};

class Vulkan_ComputePipeline {
public:
    VkPipeline computePipeline;

    Vulkan_ComputePipelineCreateInfo createInfo;
    Vulkan_PipelineLayout* pipelineLayout;

    void init(Vulkan_ComputePipelineCreateInfo& aCreateInfo);

    void destroy();

    void compile();

    void recompile();

    void bind();

    void dispatch(uint32_t threadgroupsX, uint32_t threadgroupsY, uint32_t threadgroupsZ,
                  uint32_t threadsPerGroupX, uint32_t threadsPerGroupY, uint32_t threadsPerGroupZ);
};

} //namespace lv

#endif
