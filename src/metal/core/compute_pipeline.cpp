#include "metal/lvcore/core/compute_pipeline.hpp"

#include "metal/lvcore/core/swap_chain.hpp"
#include "metal/lvcore/core/device.hpp"

namespace lv {

void Metal_ComputePipeline::init(Metal_ComputePipelineCreateInfo& aCreateInfo) {
    createInfo = aCreateInfo;

    compile();
}

void Metal_ComputePipeline::destroy() {
    computePipeline->release();
}

void Metal_ComputePipeline::compile() {
    MTL::ComputePipelineDescriptor* computePipelineDesc = MTL::ComputePipelineDescriptor::alloc()->init();
    computePipelineDesc->setThreadGroupSizeIsMultipleOfThreadExecutionWidth(true);
    computePipelineDesc->setComputeFunction(createInfo.computeShaderModule->function);
    
    NS::Error* error;
    computePipeline = g_metal_device->device->newComputePipelineState(computePipelineDesc, 0, nullptr, &error);
    if (!computePipeline) {
        throw std::runtime_error(error->localizedDescription()->utf8String());
    }

    computePipelineDesc->release();
}

void Metal_ComputePipeline::recompile() {
    computePipeline->release();
    compile();
}

void Metal_ComputePipeline::bind() {
    encoder = g_metal_swapChain->activeCommandBuffer->createComputeCommandEncoder();
    encoder->setComputePipelineState(computePipeline);
    g_metal_swapChain->activePipelineLayout = createInfo.pipelineLayout;
    g_metal_swapChain->activeComputeEncoder = encoder;
    g_metal_swapChain->activeShaderBundles[LV_SHADER_STAGE_COMPUTE_INDEX] = createInfo.computeShaderModule->shaderBundle;
}

void Metal_ComputePipeline::dispatch(uint32_t threadgroupsX, uint32_t threadgroupsY, uint32_t threadgroupsZ,
                               uint32_t threadsPerGroupX, uint32_t threadsPerGroupY, uint32_t threadsPerGroupZ) {
    encoder->dispatchThreadgroups(MTL::Size::Make(threadgroupsX, threadgroupsY, threadgroupsZ),
                                  MTL::Size::Make(threadsPerGroupX, threadsPerGroupY, threadsPerGroupZ));

    encoder->endEncoding();
}

} //namespace lv
