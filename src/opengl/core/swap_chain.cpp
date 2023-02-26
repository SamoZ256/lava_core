#include "opengl/lvcore/core/swap_chain.hpp"

#include <iostream>

#include "lvcore/common.hpp"

namespace lv {

OpenGL_SwapChain* g_opengl_swapChain = nullptr;

GLuint OpenGL_SwapChain::emptyVertexArrayObject = 0;
OpenGL_GraphicsPipeline* OpenGL_SwapChain::activeGraphicsPipeline = nullptr;

void OpenGL_SwapChainFramebuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

OpenGL_SwapChain::OpenGL_SwapChain(OpenGL_SwapChainCreateInfo& createInfo) {
    _window = createInfo.window;
    //framebuffer.clearAttachment = createInfo.clearAttachment;

    lvndOpenGLCreateContext(_window);
    if (!gladLoadGLLoader((GLADloadproc)lvndOpenGLGetLoadProc))
        LV_ERROR("Failed to initialize glad");

    glEnable(GL_FRAMEBUFFER_SRGB);

    //glEnable(GL_DEPTH_TEST);
    glFrontFace(LV_FRONT_FACE_COUNTER_CLOCKWISE);

    glGenVertexArrays(1, &emptyVertexArrayObject);

    g_opengl_swapChain = this;
}

void OpenGL_SwapChain::resize(LvndWindow* window) {
    lvndOpenGLResize(_window);
}

void OpenGL_SwapChain::renderAndPresent() {
    lvndOpenGLSwapBuffers(_window);
}

void OpenGL_SwapChain::renderFullscreenTriangle() {
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

} //namespace lv
