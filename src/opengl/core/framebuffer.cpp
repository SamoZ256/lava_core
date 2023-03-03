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
        } else {
            glFramebufferTextureLayer(
                GL_FRAMEBUFFER,
                depthBindPoint,
                depthAttachment.imageView->image->image,
                depthAttachment.imageView->baseMip,
                depthAttachment.imageView->baseLayer
            );
        }
    }

    GLenum framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (framebufferStatus != GL_FRAMEBUFFER_COMPLETE)
        LV_ERROR("Failed to create framebuffer. Status: " + std::to_string(framebufferStatus));
    
    clearValues.resize(colorAttachments.size() + (depthAttachment.index == -1 ? 0 : 1));
    for (auto& colorAttachment : colorAttachments) {
        clearValues[colorAttachment.index].color = {0.0f, 0.0f, 0.0f, 1.0f}; //TODO: pick the clear value based on format
    }

    if (depthAttachment.index != -1)
        clearValues[depthAttachment.index].depthStencil = {1.0f, 0};
}

void OpenGL_Framebuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    for (uint8_t i = 0; i < colorAttachments.size(); i++) {
        uint8_t index = colorAttachments[i].index;
        OpenGL_RenderPassAttachment* renderPassAttachment = renderPass->sortedAttachments[index];
        if (renderPassAttachment->loadOp == LV_ATTACHMENT_LOAD_OP_CLEAR)
            glClearBufferfv(GL_COLOR, index, clearValues[index].color.float32); //TODO: pick the clear value based on format
    }

    if (depthAttachment.index != -1) {
        uint8_t index = depthAttachment.index;
        OpenGL_RenderPassAttachment* renderPassAttachment = renderPass->sortedAttachments[index];
        if (renderPassAttachment->loadOp == LV_ATTACHMENT_LOAD_OP_CLEAR)
            glClearBufferfv(GL_DEPTH, index, &clearValues[index].depthStencil.depth);
        //TODO: clear the stencil as well
    }
    //TODO: remove this
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //TODO: set blending
}

} //namespace lv
