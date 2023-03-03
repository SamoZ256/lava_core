#include "metal/lvcore/core/graphics_pipeline.hpp"

#include <string>
#include <iostream>

#include "metal/lvcore/core/common.hpp"

#include "metal/lvcore/core/device.hpp"
#include "metal/lvcore/core/swap_chain.hpp"

namespace lv {

void Metal_GraphicsPipeline::init() {
    compile();
}

void Metal_GraphicsPipeline::compile() {
    MTL::RenderPipelineDescriptor* descriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    descriptor->setVertexFunction(vertexShaderModule->function);
    descriptor->setFragmentFunction(fragmentShaderModule->function);
    if (vertexDescriptor != nullptr)
        descriptor->setVertexDescriptor(vertexDescriptor->vertexDesc);
    
    descriptor->setInputPrimitiveTopology(MTL::PrimitiveTopologyClassTriangle);

    auto& subpass = renderPass->subpasses[subpassIndex];
    if (subpass->depthAttachment.index != -1) {
        for (uint8_t i = 0; i < renderPass->attachments.size(); i++) {
            if (renderPass->attachments[i].format >= LV_FORMAT_D16_UNORM && renderPass->attachments[i].format <= MTL::PixelFormatX24_Stencil8)
                descriptor->setDepthAttachmentPixelFormat(renderPass->attachments[i].format);
        }
    }

    //Setting blend states
    for (uint8_t i = 0; i < colorBlendAttachments.size(); i++) {
        Metal_RenderPassAttachment* renderPassAttachment = &renderPass->attachments[colorBlendAttachments[i].index];
        MTL::RenderPipelineColorAttachmentDescriptor* attachment = descriptor->colorAttachments()->object(i);
        attachment->setPixelFormat(renderPassAttachment->format);
        if (colorBlendAttachments[i].blendEnable) {
            attachment->setBlendingEnabled(true);
            attachment->setSourceRGBBlendFactor(colorBlendAttachments[i].srcRgbBlendFactor);
            attachment->setDestinationRGBBlendFactor(colorBlendAttachments[i].dstRgbBlendFactor);
            attachment->setRgbBlendOperation(colorBlendAttachments[i].rgbBlendOp);
            attachment->setSourceAlphaBlendFactor(colorBlendAttachments[i].srcAlphaBlendFactor);
            attachment->setDestinationAlphaBlendFactor(colorBlendAttachments[i].dstAlphaBlendFactor);
            attachment->setAlphaBlendOperation(colorBlendAttachments[i].alphaBlendOp);
        }
    }

    NS::Error* pError = nullptr;
    graphicsPipeline = g_metal_device->device->newRenderPipelineState(descriptor, &pError);
    if (!graphicsPipeline) {
        throw std::runtime_error(pError->localizedDescription()->utf8String());
    }

    descriptor->release();

    MTL::DepthStencilDescriptor* depthStencilDesc = MTL::DepthStencilDescriptor::alloc()->init();
    if (config.depthTestEnable) {
        depthStencilDesc->setDepthCompareFunction(config.depthOp);
        depthStencilDesc->setDepthWriteEnabled(config.depthWriteEnable);
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
    g_metal_swapChain->activeRenderEncoder->setCullMode(config.cullMode);
    g_metal_swapChain->activePipelineLayout = pipelineLayout;
    g_metal_swapChain->activeShaderBundles[LV_SHADER_STAGE_VERTEX_INDEX] = vertexShaderModule->shaderBundle;
    g_metal_swapChain->activeShaderBundles[LV_SHADER_STAGE_FRAGMENT_INDEX] = fragmentShaderModule->shaderBundle;
}

void Metal_GraphicsPipeline::uploadPushConstants(void* data, uint16_t index/*, size_t size, LvShaderStageFlags shaderStage*/) {
    size_t size = pipelineLayout->pushConstantRanges[index].size;
    LvShaderStageFlags shaderStage = pipelineLayout->pushConstantRanges[index].stageFlags;

    if (shaderStage & LV_SHADER_STAGE_VERTEX_BIT) {
        uint32_t bufferIndex = g_metal_swapChain->activeShaderBundles[LV_SHADER_STAGE_VERTEX_INDEX]->pushConstantBinding;
        g_metal_swapChain->activeRenderEncoder->setVertexBytes(data, roundToMultipleOf16(size), bufferIndex);
    }
    if (shaderStage & LV_SHADER_STAGE_FRAGMENT_BIT) {
        uint32_t bufferIndex = g_metal_swapChain->activeShaderBundles[LV_SHADER_STAGE_FRAGMENT_INDEX]->pushConstantBinding;
        g_metal_swapChain->activeRenderEncoder->setFragmentBytes(data, roundToMultipleOf16(size), bufferIndex);
    }
}

} //namespace lv
