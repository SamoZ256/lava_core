#ifndef LV_OPENGL_SWAP_CHAIN_H
#define LV_OPENGL_SWAP_CHAIN_H

#include <stdexcept>

#include <glad/glad.h>

#define LVND_BACKEND_OPENGL
#include "lvnd/lvnd.h"

#include "graphics_pipeline.hpp"
#include "command_buffer.hpp"

namespace lv {

class OpenGL_SwapChainFramebuffer {
public:
    //bool clearAttachment;

    void bind();

    void unbind() {}
};

struct OpenGL_SwapChainCreateInfo {
	LvndWindow* window;
  	bool vsyncEnabled = true;
	uint8_t maxFramesInFlight = 2;
	bool clearAttachment = false;
};

class OpenGL_SwapChain {
public:
    LvndWindow* _window;

    OpenGL_SwapChainFramebuffer framebuffer;
    OpenGL_CommandBuffer commandBuffer;

    LvFormat depthFormat = LV_FORMAT_D32_SFLOAT;

    static GLuint emptyVertexArrayObject;

    OpenGL_SwapChain(OpenGL_SwapChainCreateInfo& createInfo);

    void init(LvndWindow* window) {}

    void destroy() {}

    void resize(LvndWindow* window);

    void acquireNextImage() {}

    void renderAndPresent();

    void renderFullscreenTriangle();

    uint32_t width() { return _width; }

    uint32_t height() { return _height; }

    //Currently active
    static OpenGL_GraphicsPipeline* activeGraphicsPipeline;

private:
    uint32_t _width, _height;
};

extern OpenGL_SwapChain* g_opengl_swapChain;

} //namespace lv

#endif
