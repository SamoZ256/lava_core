#include "vulkan/lvcore/core/compute_pipeline.hpp"

namespace lv {

Vulkan_ComputePipeline::Vulkan_ComputePipeline(Vulkan_ComputePipelineCreateInfo& createInfo) : pipelineLayout(*createInfo.pipelineLayout) {
    VkComputePipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.stage = createInfo.computeShaderModule->stageInfo;
    pipelineInfo.layout = pipelineLayout.pipelineLayout;
    VK_CHECK_RESULT(vkCreateComputePipelines(g_vulkan_device->device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &computePipeline));

    commandBuffer.init();
}

void Vulkan_ComputePipeline::destroy() {
    vkDestroyPipeline(g_vulkan_device->device(), computePipeline, nullptr);
}

void Vulkan_ComputePipeline::bind() {
    commandBuffer.bind();
    
    vkCmdBindPipeline(g_vulkan_swapChain->getActiveCommandBuffer(), VK_PIPELINE_BIND_POINT_COMPUTE, computePipeline);
    g_vulkan_swapChain->pipelineBindPoint = VK_PIPELINE_BIND_POINT_COMPUTE;
}

void Vulkan_ComputePipeline::dispatch(uint32_t threadgroupsX, uint32_t threadgroupsY, uint32_t threadgroupsZ,
                               uint32_t threadsPerGroupX, uint32_t threadsPerGroupY, uint32_t threadsPerGroupZ) {
    vkCmdDispatch(g_vulkan_swapChain->getActiveCommandBuffer(), threadgroupsX, threadgroupsY, threadgroupsZ);

    commandBuffer.unbind();

    commandBuffer.submit();
}

} //namespace lv
