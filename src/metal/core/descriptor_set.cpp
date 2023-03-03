#include "metal/lvcore/core/descriptor_set.hpp"

#include <iostream>

#define GET_BINDING_BY_SHADER_STAGE_INDEX(shaderStageIndex, bindingsName, bindingIndicesName) \
g_metal_swapChain->activeShaderBundles[shaderStageIndex]->bindingsName[layoutIndex][bindingIndicesName[i]]

namespace lv {

/*
uint8_t shaderStageBitToIndex(LvShaderStageFlags shaderStage) {
    if (shaderStage & LV_SHADER_STAGE_VERTEX_BIT)
        return LV_SHADER_STAGE_VERTEX_INDEX;
    else if (shaderStage & LV_SHADER_STAGE_FRAGMENT_BIT)
        return LV_SHADER_STAGE_FRAGMENT_INDEX;
    else if (shaderStage & LV_SHADER_STAGE_COMPUTE_BIT)
        return LV_SHADER_STAGE_COMPUTE_INDEX;
    
    return 255;
}
*/

void Metal_DescriptorSet::addBinding(Metal_BufferInfo bufferInfo, uint32_t binding) {
    buffers.push_back(bufferInfo.buffers);
    bufferBindingIndices.push_back(binding);
}

void Metal_DescriptorSet::addBinding(Metal_ImageInfo imageInfo, uint32_t binding) {
    textures.push_back(imageInfo.imageViews);
    textureBindingIndices.push_back(binding);
    if (imageInfo.descriptorType == LV_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
        samplers.push_back(imageInfo.sampler);
        samplerBindingIndices.push_back(binding);
    }
}

void Metal_DescriptorSet::bind() {
    for (uint32_t i = 0; i < buffers.size(); i++) {
        MTL::Buffer* buffer = buffers[i][std::min(g_metal_swapChain->crntFrame, uint8_t(buffers[i].size() - 1))];
        LvShaderStageFlags shaderStage = pipelineLayout->descriptorSetLayouts[layoutIndex].bindings[bufferBindingIndices[i]].shaderStage;

        if (shaderStage & LV_SHADER_STAGE_VERTEX_BIT) {
            uint32_t binding = GET_BINDING_BY_SHADER_STAGE_INDEX(LV_SHADER_STAGE_VERTEX_INDEX, bufferBindings, bufferBindingIndices);
            g_metal_swapChain->activeRenderEncoder->setVertexBuffer(buffer, 0, binding);
        }
        if (shaderStage & LV_SHADER_STAGE_FRAGMENT_BIT) {
            uint32_t binding = GET_BINDING_BY_SHADER_STAGE_INDEX(LV_SHADER_STAGE_FRAGMENT_INDEX, bufferBindings, bufferBindingIndices);
            g_metal_swapChain->activeRenderEncoder->setFragmentBuffer(buffer, 0, binding);
        }
        if (shaderStage & LV_SHADER_STAGE_COMPUTE_BIT) {
            uint32_t binding = GET_BINDING_BY_SHADER_STAGE_INDEX(LV_SHADER_STAGE_COMPUTE_INDEX, bufferBindings, bufferBindingIndices);
            g_metal_swapChain->activeComputeEncoder->setBuffer(buffer, 0, binding);
        }
    }

    for (uint32_t i = 0; i < textures.size(); i++) {
        MTL::Texture* texture = textures[i][std::min(g_metal_swapChain->crntFrame, uint8_t(textures[i].size() - 1))];
        LvShaderStageFlags shaderStage = pipelineLayout->descriptorSetLayouts[layoutIndex].bindings[textureBindingIndices[i]].shaderStage;

        if (shaderStage & LV_SHADER_STAGE_VERTEX_BIT) {
            uint32_t binding = GET_BINDING_BY_SHADER_STAGE_INDEX(LV_SHADER_STAGE_VERTEX_INDEX, textureBindings, textureBindingIndices);
            g_metal_swapChain->activeRenderEncoder->setVertexTexture(texture, binding);
        }
        if (shaderStage & LV_SHADER_STAGE_FRAGMENT_BIT) {
            uint32_t binding = GET_BINDING_BY_SHADER_STAGE_INDEX(LV_SHADER_STAGE_FRAGMENT_INDEX, textureBindings, textureBindingIndices);
            g_metal_swapChain->activeRenderEncoder->setFragmentTexture(texture, binding);
        }
        if (shaderStage & LV_SHADER_STAGE_COMPUTE_BIT) {
            uint32_t binding = GET_BINDING_BY_SHADER_STAGE_INDEX(LV_SHADER_STAGE_COMPUTE_INDEX, textureBindings, textureBindingIndices);
            g_metal_swapChain->activeComputeEncoder->setTexture(texture, binding);
        }
    }

    for (uint32_t i = 0; i < samplers.size(); i++) {
        LvShaderStageFlags shaderStage = pipelineLayout->descriptorSetLayouts[layoutIndex].bindings[samplerBindingIndices[i]].shaderStage;

        if (shaderStage & LV_SHADER_STAGE_VERTEX_BIT) {
            uint32_t binding = GET_BINDING_BY_SHADER_STAGE_INDEX(LV_SHADER_STAGE_VERTEX_INDEX, samplerBindings, samplerBindingIndices);
            g_metal_swapChain->activeRenderEncoder->setVertexSamplerState(samplers[i], binding);
        }
        if (shaderStage & LV_SHADER_STAGE_FRAGMENT_BIT) {
            uint32_t binding = GET_BINDING_BY_SHADER_STAGE_INDEX(LV_SHADER_STAGE_FRAGMENT_INDEX, samplerBindings, samplerBindingIndices);
            g_metal_swapChain->activeRenderEncoder->setFragmentSamplerState(samplers[i], binding);
        }
        if (shaderStage & LV_SHADER_STAGE_COMPUTE_BIT) {
            uint32_t binding = GET_BINDING_BY_SHADER_STAGE_INDEX(LV_SHADER_STAGE_COMPUTE_INDEX, samplerBindings, samplerBindingIndices);
            g_metal_swapChain->activeComputeEncoder->setSamplerState(samplers[i], binding);
        }
    }
}

} //namespace lv
