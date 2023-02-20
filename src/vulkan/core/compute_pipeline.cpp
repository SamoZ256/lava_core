#include "vulkan/lvcore/core/compute_pipeline.hpp"

namespace lv {

void Vulkan_ComputePipeline::init(Vulkan_ComputePipelineCreateInfo& aCreateInfo) {
    createInfo = aCreateInfo;
    pipelineLayout = createInfo.pipelineLayout;

    compile();
}

void Vulkan_ComputePipeline::destroy() {
    vkDestroyPipeline(g_vulkan_device->device(), computePipeline, nullptr);
}

void Vulkan_ComputePipeline::compile() {
    VkComputePipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.stage = createInfo.computeShaderModule->stageInfo;
    pipelineInfo.layout = pipelineLayout->pipelineLayout;
    VK_CHECK_RESULT(vkCreateComputePipelines(g_vulkan_device->device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &computePipeline));
}

void Vulkan_ComputePipeline::recompile() {
    destroy();
    compile();
}

void Vulkan_ComputePipeline::bind() {
    
    vkCmdBindPipeline(g_vulkan_swapChain->getActiveCommandBuffer(), VK_PIPELINE_BIND_POINT_COMPUTE, computePipeline);
    g_vulkan_swapChain->pipelineBindPoint = VK_PIPELINE_BIND_POINT_COMPUTE;
}

void Vulkan_ComputePipeline::dispatch(uint32_t threadgroupsX, uint32_t threadgroupsY, uint32_t threadgroupsZ,
                               uint32_t threadsPerGroupX, uint32_t threadsPerGroupY, uint32_t threadsPerGroupZ) {
    vkCmdDispatch(g_vulkan_swapChain->getActiveCommandBuffer(), threadgroupsX, threadgroupsY, threadgroupsZ);
}

} //namespace lv
