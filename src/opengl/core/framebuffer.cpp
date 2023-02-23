#include "opengl/lvcore/core/framebuffer.hpp"

namespace lv {

void OpenGL_Framebuffer::init(OpenGL_RenderPass* aRenderPass) {
    renderPass = aRenderPass;

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    
    for (uint8_t i = 0; i < colorAttachments.size(); i++) {
        if (colorAttachments[i].imageView->layerCount > 1) {
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
    }

    if (depthAttachment.index != -1) {
        GLenum depthBindPoint = 0;
        if (depthAttachment.imageView->image->aspectMask & LV_IMAGE_ASPECT_DEPTH_BIT & LV_IMAGE_ASPECT_STENCIL_BIT)
            depthBindPoint = GL_DEPTH_STENCIL_ATTACHMENT;
        else if (depthAttachment.imageView->image->aspectMask & LV_IMAGE_ASPECT_DEPTH_BIT)
            depthBindPoint = GL_DEPTH_ATTACHMENT;
        else if (depthAttachment.imageView->image->aspectMask & LV_IMAGE_ASPECT_STENCIL_BIT)
            depthBindPoint = GL_STENCIL_ATTACHMENT;
        if (depthAttachment.imageView->layerCount > 1) {
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
                depthAttachment.imageView->viewType,
                depthAttachment.imageView->baseMip,
                depthAttachment.imageView->baseLayer
            );
        }
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("Framebuffer::init: Failed to create framebuffer");
}

void OpenGL_Framebuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    //TODO: clear attachments
    //TODO: set blending
}

} //namespace lv
