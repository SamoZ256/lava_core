#include "opengl/lvcore/core/swap_chain.hpp"

namespace lv {

OpenGL_SwapChain* g_opengl_swapChain = nullptr;

OpenGL_SwapChain::OpenGL_SwapChain(OpenGL_SwapChainCreateInfo& createInfo) {
    _window = createInfo.window;

    g_opengl_swapChain = this;
}

} //namespace lv
