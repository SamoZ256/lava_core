#ifndef LV_METAL_COMPUTE_PIPELINE_H
#define LV_METAL_COMPUTE_PIPELINE_H

#include "shader_module.hpp"

namespace lv {

struct Metal_ComputePipelineCreateInfo {
    Metal_ShaderModule* computeShaderModule;
};

class Metal_ComputePipeline {
public:
    MTL::ComputePipelineState* computePipeline;
    Metal_CommandBuffer commandBuffer;
    MTL::ComputeCommandEncoder* encoder;

    Metal_ComputePipeline(Metal_ComputePipelineCreateInfo& createInfo);

    void destroy();

    void bind();

    void dispatch(uint32_t threadgroupsX, uint32_t threadgroupsY, uint32_t threadgroupsZ,
                  uint32_t threadsPerGroupX, uint32_t threadsPerGroupY, uint32_t threadsPerGroupZ);
};

} //namespace lv

#endif
