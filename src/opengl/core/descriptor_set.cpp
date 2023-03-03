#include "opengl/lvcore/core/descriptor_set.hpp"

#include <iostream>

#include "opengl/lvcore/core/swap_chain.hpp"

namespace lv {

void OpenGL_DescriptorSet::bind() {
    for (uint32_t i = 0; i < bufferInfos.size(); i++) {
        uint16_t bufferBinding = g_opengl_swapChain->activeGraphicsPipeline->bufferBindings[layoutIndex][bufferBindingIndices[i]] + 2; //offset the binding index by 2 to provide space for push constants
        if (firstTimeBound)
            glUniformBlockBinding(g_opengl_swapChain->activeGraphicsPipeline->shaderProgram, g_opengl_swapChain->activeGraphicsPipeline->bindings[layoutIndex][bufferBindingIndices[i]], bufferBinding);
        glBindBuffer(GL_UNIFORM_BUFFER, bufferInfos[i].buffer);
        glBindBufferBase(GL_UNIFORM_BUFFER, bufferBinding, bufferInfos[i].buffer);
        //std::cout << "Buffer binding: " << g_opengl_swapChain->activeGraphicsPipeline->bindings[layoutIndex][bufferBindingIndices[i]] << " : " << bufferBinding << " : " << bufferBindingIndices[i] << std::endl;
    }

    for (uint32_t i = 0; i < imageInfos.size(); i++) {
        uint16_t textureBinding = g_opengl_swapChain->activeGraphicsPipeline->textureBindings[layoutIndex][imageBindingIndices[i]];
        if (firstTimeBound)
            glUniform1i(g_opengl_swapChain->activeGraphicsPipeline->bindings[layoutIndex][imageBindingIndices[i]], textureBinding);
        glActiveTexture(GL_TEXTURE0 + textureBinding);
        glBindTexture(imageInfos[i].viewType, imageInfos[i].image);
        if (imageInfos[i].descriptorType == LV_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
            glBindSampler(textureBinding, imageInfos[i].sampler);
    }

    firstTimeBound = false;
}

} //namespace lv
