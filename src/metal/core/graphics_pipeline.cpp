#include "lvcore/core/graphics_pipeline.hpp"

#include <string>

#include "lvcore/core/device.hpp"

namespace lv {

GraphicsPipeline::GraphicsPipeline(GraphicsPipelineCreateInfo& createInfo) {
    MTL::RenderPipelineDescriptor* descriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    descriptor->setVertexFunction(createInfo.vertexShaderModule->function);
    descriptor->setFragmentFunction(createInfo.fragmentShaderModule->function);
    //descriptor->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
    if (createInfo.vertexDescriptor != nullptr)
        descriptor->setVertexDescriptor(createInfo.vertexDescriptor->vertexDesc);
    
    descriptor->setInputPrimitiveTopology(MTL::PrimitiveTopologyClassTriangle);
    if (createInfo.framebuffer != nullptr) {
        if (createInfo.framebuffer->depthAttachment.image != nullptr)
            descriptor->setDepthAttachmentPixelFormat(createInfo.framebuffer->depthAttachment.image->format);
        for (auto& attachment : createInfo.framebuffer->colorAttachments) {
            descriptor->colorAttachments()->object(attachment.attachmentIndex)->setPixelFormat(attachment.image->format);
        }
    }

    //Setting blend states
    for (auto& colorAttachment : createInfo.framebuffer->colorAttachments) {
        uint8_t blendEnable = createInfo.config.blends[0];
        if (colorAttachment.attachmentIndex < createInfo.config.blends.size())
            blendEnable = createInfo.config.blends[colorAttachment.attachmentIndex];
        MTL::RenderPipelineColorAttachmentDescriptor* attachment = descriptor->colorAttachments()->object(colorAttachment.attachmentIndex);
        attachment->setPixelFormat(colorAttachment.image->format);
        if (blendEnable) {
            for (uint8_t i = 0; i < createInfo.framebuffer->frameCount; i++) {
                attachment->setBlendingEnabled(true);
                attachment->setSourceRGBBlendFactor(MTL::BlendFactorSourceAlpha);
                attachment->setDestinationRGBBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
                attachment->setRgbBlendOperation(MTL::BlendOperationAdd);
                attachment->setSourceAlphaBlendFactor(MTL::BlendFactorOne);
                attachment->setDestinationAlphaBlendFactor(MTL::BlendFactorOne);
                attachment->setAlphaBlendOperation(MTL::BlendOperationMax);
            }
        }
    }

    NS::Error* pError = nullptr;
    graphicsPipeline = g_device->device->newRenderPipelineState(descriptor, &pError);
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

    depthStencilState = g_device->device->newDepthStencilState(depthStencilDesc);

    cullMode = createInfo.config.cullMode;
}

void GraphicsPipeline::bind() {
    g_swapChain->activeRenderEncoder->setRenderPipelineState(graphicsPipeline);

    g_swapChain->activeRenderEncoder->setDepthStencilState(depthStencilState);
    g_swapChain->activeRenderEncoder->setFrontFacingWinding(MTL::WindingCounterClockwise);
    g_swapChain->activeRenderEncoder->setCullMode(cullMode);
}

void GraphicsPipeline::uploadPushConstants(void* data, uint16_t index, size_t size, LvShaderStage shaderStage) {
    if (shaderStage == LV_SHADER_STAGE_VERTEX_BIT)
        g_swapChain->activeRenderEncoder->setVertexBytes(data, size, index);
    else if (shaderStage == LV_SHADER_STAGE_FRAGMENT_BIT)
        g_swapChain->activeRenderEncoder->setFragmentBytes(data, size, index);
    else
        throw std::runtime_error("GraphicsPipeline::uploadPushConstants: invalid shader stage '" + std::to_string(shaderStage) + "'");
}

} //namespace lv
