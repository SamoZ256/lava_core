#include "metal/lvcore/core/compute_pipeline.hpp"

#include "metal/lvcore/core/swap_chain.hpp"
#include "metal/lvcore/core/device.hpp"

namespace lv {

Metal_ComputePipeline::Metal_ComputePipeline(Metal_ComputePipelineCreateInfo& createInfo) {
    MTL::ComputePipelineDescriptor* computePipelineDesc = MTL::ComputePipelineDescriptor::alloc()->init();
    computePipelineDesc->setThreadGroupSizeIsMultipleOfThreadExecutionWidth(true);
    computePipelineDesc->setComputeFunction(createInfo.computeShaderModule->function);
    
    NS::Error* error;
    computePipeline = g_metal_device->device->newComputePipelineState(computePipelineDesc, 0, nullptr, &error);
    if (!computePipeline) {
        throw std::runtime_error(error->localizedDescription()->utf8String());
    }

    computePipelineDesc->release();

    commandBuffer.init();
}

void Metal_ComputePipeline::destroy() {
    computePipeline->release();
}

void Metal_ComputePipeline::bind() {
    commandBuffer.bind();
    encoder = commandBuffer.createComputeCommandEncoder();
    encoder->setComputePipelineState(computePipeline);
    g_metal_swapChain->activeComputeEncoder = encoder;
}

void Metal_ComputePipeline::dispatch(uint32_t threadgroupsX, uint32_t threadgroupsY, uint32_t threadgroupsZ,
                               uint32_t threadsPerGroupX, uint32_t threadsPerGroupY, uint32_t threadsPerGroupZ) {
    encoder->dispatchThreadgroups(MTL::Size::Make(threadgroupsX, threadgroupsY, threadgroupsZ),
                                  MTL::Size::Make(threadsPerGroupX, threadsPerGroupY, threadsPerGroupZ));

    encoder->endEncoding();

    commandBuffer.submit();
    //encoder->release();
}

} //namespace lv
