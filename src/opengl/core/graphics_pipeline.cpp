#include "opengl/lvcore/core/graphics_pipeline.hpp"

#include <string>
#include <iostream>

#include "lvcore/common.hpp"

#include "opengl/lvcore/core/swap_chain.hpp"

namespace lv {

void OpenGL_GraphicsPipeline::init() {
    compile();
}

void OpenGL_GraphicsPipeline::destroy() {
    glDeleteProgram(shaderProgram);
}

void OpenGL_GraphicsPipeline::compile() {
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShaderModule->shader);
    glAttachShader(shaderProgram, fragmentShaderModule->shader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        LV_ERROR("Failed to compile graphics pipeline. Log:\n" + infoLog);
    }

    //Get the bindings
    OpenGL_ShaderBundle* shaderBundles[2] = {vertexShaderModule->shaderBundle, fragmentShaderModule->shaderBundle};

    uint16_t bufferBinding = 0;
    uint16_t textureBinding = 0;
    uint32_t descriptorSetCount = 0;
    uint32_t* bindingCounts;
    for (uint8_t i = 0; i < 2; i++)
        descriptorSetCount = std::max(descriptorSetCount, shaderBundles[i]->descriptorSetCount);
    bindings = (GLuint**)malloc(sizeof(GLuint*) * descriptorSetCount);
    bufferBindings = (uint16_t**)malloc(sizeof(uint16_t*) * descriptorSetCount);
    textureBindings = (uint16_t**)malloc(sizeof(uint16_t*) * descriptorSetCount);
    bindingCounts = (uint32_t*)malloc(sizeof(uint32_t) * descriptorSetCount);
    for (uint32_t i = 0; i < descriptorSetCount; i++)
        bindingCounts[i] = 0;
    for (uint8_t i = 0; i < 2; i++) {
        OpenGL_ShaderBundle* shaderBundle = shaderBundles[i];
        for (uint32_t set = 0; set < shaderBundle->descriptorSetCount; set++) {
            bindingCounts[set] = std::max(bindingCounts[set], shaderBundles[i]->bindingCounts[set]);
        }
    }
    for (uint32_t set = 0; set < descriptorSetCount; set++) {
        bindings[set] = (GLuint*)malloc(sizeof(GLuint) * bindingCounts[set]);
        bufferBindings[set] = (uint16_t*)malloc(sizeof(uint16_t) * bindingCounts[set]);
        textureBindings[set] = (uint16_t*)malloc(sizeof(uint16_t) * bindingCounts[set]);
    }
    
    //Push constants
    for (uint8_t i = 0; i < pipelineLayout->pushConstantRanges.size(); i++) {
        for (uint8_t j = 0; j < 2; j++) {
            if (shaderBundles[j]->pushConstantBinding) {
                OpenGL_PushConstantBinding pushConstantBinding;
                pushConstantBinding.location = glGetUniformBlockIndex(shaderProgram, shaderBundles[j]->pushConstantBinding->c_str());
                std::cout << "Name: " << shaderBundles[j]->pushConstantBinding->c_str() << std::endl;
                pushConstantBinding.uniformBuffer.usage = LV_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
                pushConstantBinding.uniformBuffer.memoryType = LV_MEMORY_TYPE_SHARED;
                pushConstantBinding.uniformBuffer.init(pipelineLayout->pushConstantRanges[i].size);
                //glUniformBlockBinding(shaderProgram, pushConstantBinding.location, i);

                pushConstantBindings.push_back(pushConstantBinding);
                break;
            }
        }
    }

    for (uint8_t i = 0; i < 2; i++) {
        OpenGL_ShaderBundle* shaderBundle = shaderBundles[i];
        
        for (uint32_t set = 0; set < shaderBundle->descriptorSetCount; set++) {
            for (uint32_t binding = 0; binding < shaderBundle->bindingCounts[set]; binding++) {
                OpenGL_ShaderBinding& shaderBinding = shaderBundle->bindings[set][binding];
                if (shaderBinding.exists) {
                    //std::cout << "Set " << set << " binding " << binding << ": " << shaderBinding.name << std::endl;
                    switch (shaderBinding.descriptorType) {
                        case LV_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
                            bindings[set][binding] = glGetUniformBlockIndex(shaderProgram, shaderBinding.name.c_str());
                            bufferBindings[set][binding] = bufferBinding++;
                            break;
                        case LV_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
                            bindings[set][binding] = glGetUniformLocation(shaderProgram, shaderBinding.name.c_str());
                            textureBindings[set][binding] = textureBinding++;
                            std::cout << "Sampler (" << shaderBinding.name << ") location: " << bindings[set][binding] << std::endl;
                            break;
                        default:
                            LV_ERROR("Unknown descriptor type");
                    }
                    //std::cout <<"Uniform location: " << bindings[set][binding] << std::endl;
                }
            }
        }
    }
}

void OpenGL_GraphicsPipeline::recompile() {
    destroy();
    compile();
}

void OpenGL_GraphicsPipeline::bind() {
    glUseProgram(shaderProgram);

    if (!vertexDescriptor)
        glBindVertexArray(g_opengl_swapChain->emptyVertexArrayObject);
    
    for (uint8_t i = 0; i < pushConstantBindings.size(); i++) {
        glBindBuffer(GL_UNIFORM_BUFFER, pushConstantBindings[i].uniformBuffer.buffer);
        glBindBufferBase(GL_UNIFORM_BUFFER, i, pushConstantBindings[i].uniformBuffer.buffer);
        //std::cout << "Uniform block 1: " << glGetError() << std::endl;
        glUniformBlockBinding(shaderProgram, pushConstantBindings[i].location, i);
        //std::cout << "Location: " << pushConstantBindings[i].location << ", Index: " << (int)i << std::endl;
        //std::cout << "Uniform block 2: " << glGetError() << std::endl;
    }

    glDepthMask(config.depthWriteEnable);
    if (config.depthTestEnable) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(config.depthOp);
    } else {
        glDisable(GL_DEPTH_TEST);
    }

    if (config.cullMode == LV_CULL_MODE_NONE) {
        glDisable(GL_CULL_FACE);
    } else {
        glEnable(GL_CULL_FACE);
        glCullFace(config.cullMode);
    }

    for (uint8_t i = 0; i < colorBlendAttachments.size(); i++) {
        if (colorBlendAttachments[i].blendEnable) {
            glBlendFuncSeparate(colorBlendAttachments[i].srcRgbBlendFactor, colorBlendAttachments[i].dstRgbBlendFactor, colorBlendAttachments[i].srcAlphaBlendFactor, colorBlendAttachments[i].dstAlphaBlendFactor);
            glBlendEquationSeparate(colorBlendAttachments[i].rgbBlendOp, colorBlendAttachments[i].alphaBlendOp);
        }
    }
    
    g_opengl_swapChain->activeGraphicsPipeline = this;
}

void OpenGL_GraphicsPipeline::uploadPushConstants(void* data, uint16_t index) {
    pushConstantBindings[index].uniformBuffer.copyDataTo(0, data);
}

} //namespace lv
