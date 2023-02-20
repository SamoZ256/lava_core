#include "metal/lvcore/core/graphics_pipeline.hpp"

#include <string>

#include "metal/lvcore/core/common.hpp"

#include "metal/lvcore/core/device.hpp"
#include "metal/lvcore/core/swap_chain.hpp"

namespace lv {

void Metal_GraphicsPipeline::init(Metal_GraphicsPipelineCreateInfo& aCreateInfo) {
    createInfo = aCreateInfo;

    compile();

    cullMode = createInfo.config.cullMode;
}

void Metal_GraphicsPipeline::compile() {
    MTL::RenderPipelineDescriptor* descriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    descriptor->setVertexFunction(createInfo.vertexShaderModule->function);
    descriptor->setFragmentFunction(createInfo.fragmentShaderModule->function);
    //descriptor->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
    if (createInfo.vertexDescriptor != nullptr)
        descriptor->setVertexDescriptor(createInfo.vertexDescriptor->vertexDesc);
    
    descriptor->setInputPrimitiveTopology(MTL::PrimitiveTopologyClassTriangle);
    if (createInfo.renderPass->depthAttachment.index != -1)
        descriptor->setDepthAttachmentPixelFormat(createInfo.renderPass->depthAttachment.format);
    for (uint8_t i = 0; i < createInfo.renderPass->colorAttachments.size(); i++) {
        Metal_RenderPassAttachment* renderPassAttachment = &createInfo.renderPass->colorAttachments[createInfo.renderPass->colorAttachments[i].index];
        descriptor->colorAttachments()->object(renderPassAttachment->index)->setPixelFormat(renderPassAttachment->format);
    }

    //Setting blend states
    for (uint8_t i = 0; i < createInfo.renderPass->colorAttachments.size(); i++) {
        Metal_RenderPassAttachment* renderPassAttachment = &createInfo.renderPass->colorAttachments[createInfo.renderPass->colorAttachments[i].index];
        MTL::RenderPipelineColorAttachmentDescriptor* attachment = descriptor->colorAttachments()->object(renderPassAttachment->index);
        //attachment->setPixelFormat(renderPassAttachment->format);
        if (renderPassAttachment->blendEnable) {
            attachment->setBlendingEnabled(true);
            attachment->setSourceRGBBlendFactor(renderPassAttachment->srcRgbBlendFactor);
            attachment->setDestinationRGBBlendFactor(renderPassAttachment->dstRgbBlendFactor);
            attachment->setRgbBlendOperation(renderPassAttachment->rgbBlendOp);
            attachment->setSourceAlphaBlendFactor(renderPassAttachment->srcAlphaBlendFactor);
            attachment->setDestinationAlphaBlendFactor(renderPassAttachment->dstAlphaBlendFactor);
            attachment->setAlphaBlendOperation(renderPassAttachment->alphaBlendOp);
        }
    }

    NS::Error* pError = nullptr;
    graphicsPipeline = g_metal_device->device->newRenderPipelineState(descriptor, &pError);
    if (!graphicsPipeline) {
        throw std::runtime_error(pError->localizedDescription()->utf8String());
    }

    descriptor->release();

    MTL::DepthStencilDescriptor* depthStencilDesc = MTL::DepthStencilDescriptor::alloc()->init();
    if (createInfo.config.depthTestEnable) {
        depthStencilDesc->setDepthCompareFunction(createInfo.config.depthOp);
        depthStencilDesc->setDepthWriteEnabled(createInfo.config.depthWriteEnable);
    }/* else {
        depthStencilDesc->setDepthWriteEnabled(false);
    }*/

    depthStencilState = g_metal_device->device->newDepthStencilState(depthStencilDesc);
}

void Metal_GraphicsPipeline::recompile() {
    destroy();
    compile();
}

void Metal_GraphicsPipeline::bind() {
    g_metal_swapChain->activeRenderEncoder->setRenderPipelineState(graphicsPipeline);

    g_metal_swapChain->activeRenderEncoder->setDepthStencilState(depthStencilState);
    g_metal_swapChain->activeRenderEncoder->setFrontFacingWinding(MTL::WindingCounterClockwise);
    g_metal_swapChain->activeRenderEncoder->setCullMode(cullMode);
    g_metal_swapChain->activePipelineLayout = createInfo.pipelineLayout;
    g_metal_swapChain->activeShaderBundles[LV_SHADER_STAGE_VERTEX_INDEX] = createInfo.vertexShaderModule->shaderBundle;
    g_metal_swapChain->activeShaderBundles[LV_SHADER_STAGE_FRAGMENT_INDEX] = createInfo.fragmentShaderModule->shaderBundle;
}

void Metal_GraphicsPipeline::uploadPushConstants(void* data, uint16_t index, size_t size, LvShaderStage shaderStage) {
    if (shaderStage == LV_SHADER_STAGE_VERTEX_BIT)
        g_metal_swapChain->activeRenderEncoder->setVertexBytes(data, roundToMultipleOf16(size), index);
    else if (shaderStage == LV_SHADER_STAGE_FRAGMENT_BIT)
        g_metal_swapChain->activeRenderEncoder->setFragmentBytes(data, roundToMultipleOf16(size), index);
    else
        throw std::runtime_error("GraphicsPipeline::uploadPushConstants: invalid shader stage '" + std::to_string(shaderStage) + "'");
}

} //namespace lv
