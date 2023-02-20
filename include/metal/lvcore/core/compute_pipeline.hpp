#ifndef LV_METAL_COMPUTE_PIPELINE_H
#define LV_METAL_COMPUTE_PIPELINE_H

#include "shader_module.hpp"
#include "pipeline_layout.hpp"

namespace lv {

struct Metal_ComputePipelineCreateInfo {
    Metal_ShaderModule* computeShaderModule;
    Metal_PipelineLayout* pipelineLayout;
};

class Metal_ComputePipeline {
public:
    MTL::ComputePipelineState* computePipeline = nullptr;
    MTL::ComputeCommandEncoder* encoder;

    Metal_ComputePipelineCreateInfo createInfo;

    void init(Metal_ComputePipelineCreateInfo& aCreateInfo);

    void destroy();

    void compile();

    void recompile();

    void bind();

    void dispatch(uint32_t threadgroupsX, uint32_t threadgroupsY, uint32_t threadgroupsZ,
                  uint32_t threadsPerGroupX, uint32_t threadsPerGroupY, uint32_t threadsPerGroupZ);
};

} //namespace lv

#endif
