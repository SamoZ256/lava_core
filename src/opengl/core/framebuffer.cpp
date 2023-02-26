#include "opengl/lvcore/core/framebuffer.hpp"

#include <iostream>

#include "lvcore/common.hpp"

namespace lv {

void OpenGL_Framebuffer::init(OpenGL_RenderPass* aRenderPass) {
    renderPass = aRenderPass;

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    
    std::vector<GLenum> drawBuffers;
    for (uint8_t i = 0; i < colorAttachments.size(); i++) {
        if (colorAttachments[i].imageView->baseLayer == 0) {
            glFramebufferTexture(
                GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0 + colorAttachments[i].index,
                colorAttachments[i].imageView->image->image,
                colorAttachments[i].imageView->baseMip
            );
        } else {
            glFramebufferTextureLayer(
                GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0 + colorAttachments[i].index,
                colorAttachments[i].imageView->viewType,
                colorAttachments[i].imageView->baseMip,
                colorAttachments[i].imageView->baseLayer
            );
        }
        drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + colorAttachments[i].index);
    }
    glDrawBuffers(drawBuffers.size(), drawBuffers.data());

    if (depthAttachment.index != -1) {
        GLenum depthBindPoint = 0;
        if (depthAttachment.imageView->image->aspectMask & LV_IMAGE_ASPECT_DEPTH_BIT & LV_IMAGE_ASPECT_STENCIL_BIT)
            depthBindPoint = GL_DEPTH_STENCIL_ATTACHMENT;
        else if (depthAttachment.imageView->image->aspectMask & LV_IMAGE_ASPECT_DEPTH_BIT)
            depthBindPoint = GL_DEPTH_ATTACHMENT;
        else if (depthAttachment.imageView->image->aspectMask & LV_IMAGE_ASPECT_STENCIL_BIT)
            depthBindPoint = GL_STENCIL_ATTACHMENT;
        if (depthAttachment.imageView->baseLayer == 0) {
            glFramebufferTexture(
                GL_FRAMEBUFFER,
                depthBindPoint,
                depthAttachment.imageView->image->image,
                depthAttachment.imageView->baseMip
            );
            std::cout<< "Depth1: " << glGetError() << std::endl;
        } else {
            glFramebufferTextureLayer(
                GL_FRAMEBUFFER,
                depthBindPoint,
                depthAttachment.imageView->image->image,
                depthAttachment.imageView->baseMip,
                depthAttachment.imageView->baseLayer
            );
            std::cout<< "Depth2: " << glGetError() << std::endl;
        }
    }
    std::cout << "Framebuffer: " << glGetError() << std::endl;

    //TODO: add input attachments
    if (inputAttachments.size() != 0)
        LV_ERROR("OpenGL backend does not support input attachments right now");

    GLenum framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (framebufferStatus != GL_FRAMEBUFFER_COMPLETE)
        LV_ERROR("Failed to create framebuffer. Status: " + std::to_string(framebufferStatus));
    
    clearValues.resize(colorAttachments.size() + (depthAttachment.index == -1 ? 0 : 1));
    for (auto& colorAttachment : colorAttachments) {
        clearValues[colorAttachment.index].r = 0.0f;
        clearValues[colorAttachment.index].g = 0.0f;
        clearValues[colorAttachment.index].b = 0.0f;
        clearValues[colorAttachment.index].a = 0.0f;
    }

    if (depthAttachment.index != -1)
        clearValues[depthAttachment.index].depth = 1.0f;
        clearValues[depthAttachment.index].stencil = 0;
}

void OpenGL_Framebuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    for (uint8_t i = 0; i < colorAttachments.size(); i++) {
        uint8_t index = colorAttachments[i].index;
        OpenGL_RenderPassAttachment* renderPassAttachment = renderPass->sortedAttachments[index];
        if (renderPassAttachment->loadOp == LV_ATTACHMENT_LOAD_OP_CLEAR)
            glClearBufferfv(GL_COLOR, index, &clearValues[index].r);
        if (renderPassAttachment->blendEnable) {
            glBlendFuncSeparate(renderPassAttachment->srcRgbBlendFactor, renderPassAttachment->dstRgbBlendFactor, renderPassAttachment->srcAlphaBlendFactor, renderPassAttachment->dstAlphaBlendFactor);
            glBlendEquationSeparate(renderPassAttachment->rgbBlendOp, renderPassAttachment->alphaBlendOp);
        }
    }

    if (depthAttachment.index != -1) {
        uint8_t index = depthAttachment.index;
        OpenGL_RenderPassAttachment* renderPassAttachment = renderPass->sortedAttachments[index];
        if (renderPassAttachment->loadOp == LV_ATTACHMENT_LOAD_OP_CLEAR)
            glClearBufferfv(GL_DEPTH, index, &clearValues[index].depth);
        //TODO: clear the stencil as well
    }
    //TODO: remove this
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //TODO: set blending
}

} //namespace lv
