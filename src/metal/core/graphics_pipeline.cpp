#include "lvcore/core/graphics_pipeline.hpp"

#include "lvcore/core/device.hpp"
#include "lvcore/filesystem/filesystem.hpp"

namespace lv {

ShaderModule::ShaderModule(ShaderModuleCreateInfo& createInfo) {
    std::string source = readFile(createInfo.filename);
    //std::cout << createInfo.filename/* << " : " << source*/ << std::endl;

    dispatch_data_t sourceData = convertStringToDispatchData(source);

    NS::Error* pError = nullptr;
    library = g_device->device->newLibrary(sourceData, &pError);
    //g_device->device->newLibrary()
    if (!library) {
        throw std::runtime_error(pError->localizedDescription()->utf8String());
    }

    if (createInfo.specializationConstants.size() == 0) {
        function = library->newFunction(NS::String::string(createInfo.functionName, NS::UTF8StringEncoding));
    } else {
        MTL::FunctionConstantValues* constantValues = MTL::FunctionConstantValues::alloc()->init();
        for (auto& mapEntry : createInfo.specializationConstants) {
            constantValues->setConstantValue(mapEntry.data, mapEntry.dataType, mapEntry.constantID);
        }

        MTL::FunctionDescriptor* functionDesc = MTL::FunctionDescriptor::alloc()->init();
        functionDesc->setName(NS::String::string(createInfo.functionName, NS::UTF8StringEncoding));
        functionDesc->setConstantValues(constantValues);

        //function = library->newFunction(NS::String::string(createInfo.functionName, NS::StringEncoding::UTF8StringEncoding));
        function = library->newFunction(functionDesc, &pError);
    }
}

void ShaderModule::destroy() {
    library->release();
    function->release();
}

GraphicsPipeline::GraphicsPipeline(GraphicsPipelineCreateInfo& createInfo) {
    MTL::RenderPipelineDescriptor* descriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    descriptor->setVertexFunction(createInfo.vertexShaderModule->function);
    descriptor->setFragmentFunction(createInfo.fragmentShaderModule->function);
    //descriptor->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
    if (createInfo.vertexDescriptor != nullptr)
        descriptor->setVertexDescriptor(createInfo.vertexDescriptor->vertexDesc);
    
    descriptor->setInputPrimitiveTopology(MTL::PrimitiveTopologyClassTriangle);
    if (createInfo.framebuffer != nullptr) {
        if (createInfo.framebuffer->depthAttachment.attachmentIndex != -1)
            descriptor->setDepthAttachmentPixelFormat(createInfo.framebuffer->depthAttachment.image->format);
        for (uint8_t i = 0; i < createInfo.framebuffer->colorAttachments.size(); i++) {
            descriptor->colorAttachments()->object(i)->setPixelFormat(createInfo.framebuffer->colorAttachments[i].image->format);
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
    if (createInfo.config.depthTest) {
        depthStencilDesc->setDepthCompareFunction(createInfo.config.depthOp);
        depthStencilDesc->setDepthWriteEnabled(createInfo.config.depthWrite);
    }/* else {
        depthStencilDesc->setDepthWriteEnabled(false);
    }*/

    depthStencilState = g_device->device->newDepthStencilState(depthStencilDesc);

    cullMode = createInfo.config.cullMode;
}

void GraphicsPipeline::bind() {
    g_swapChain->activeFramebuffer->encoder->setRenderPipelineState(graphicsPipeline);

    g_swapChain->activeFramebuffer->encoder->setDepthStencilState(depthStencilState);
    g_swapChain->activeFramebuffer->encoder->setFrontFacingWinding(MTL::WindingCounterClockwise);
    g_swapChain->activeFramebuffer->encoder->setCullMode(cullMode);
}

void GraphicsPipeline::uploadPushConstants(void* data, uint16_t index, size_t size, LvShaderStage shaderStage) {
    if (shaderStage == LV_SHADER_STAGE_VERTEX_BIT)
        g_swapChain->activeFramebuffer->encoder->setVertexBytes(data, size, index);
    else if (shaderStage == LV_SHADER_STAGE_FRAGMENT_BIT)
        g_swapChain->activeFramebuffer->encoder->setFragmentBytes(data, size, index);
    else
        throw std::runtime_error("GraphicsPipeline::uploadPushConstants: invalid shader stage '" + std::to_string(shaderStage) + "'");
}

} //namespace lv
