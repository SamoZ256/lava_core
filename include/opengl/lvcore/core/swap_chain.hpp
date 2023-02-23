#ifndef LV_OPENGL_SWAP_CHAIN_H
#define LV_OPENGL_SWAP_CHAIN_H

#include <stdexcept>

#include <glad/glad.h>

#define LVND_BACKEND_OPENGL
#include "lvnd/lvnd.h"

#include "graphics_pipeline.hpp"
#include "command_buffer.hpp"
#include "framebuffer.hpp"

namespace lv {

struct OpenGL_SwapChainCreateInfo {
	LvndWindow* window;
  	bool vsyncEnabled = true;
	uint8_t maxFramesInFlight = 2;
	bool clearAttachment = false;
};

class OpenGL_SwapChain {
public:
    LvndWindow* _window;

    OpenGL_Framebuffer framebuffer;
    OpenGL_CommandBuffer commandBuffer;

    OpenGL_SwapChain(OpenGL_SwapChainCreateInfo& createInfo);

    void init(LvndWindow* window) {}

    void destroy() {}

    void resize(LvndWindow* window) { lvndOpenGLResize(_window); }

    void acquireNextImage() {}

    void renderAndPresent() { lvndOpenGLSwapBuffers(_window); }

    void renderFullscreenTriangle() { glDrawArrays(GL_TRIANGLES, 0, 3); }

    uint32_t width() { return _width; }

    uint32_t height() { return _height; }

    //Currently active
    //TODO: add this

private:
    uint32_t _width, _height;
};

extern OpenGL_SwapChain* g_opengl_swapChain;

} //namespace lv

#endif
