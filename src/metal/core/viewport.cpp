#include "metal/lvcore/core/viewport.hpp"

#include <iostream>

namespace lv {

Metal_Viewport::Metal_Viewport(int32_t x, int32_t y, uint32_t width, uint32_t height) {
    setViewport(x, y, width, height);
}

void Metal_Viewport::setViewport(int32_t x, int32_t y, uint32_t width, uint32_t height) {
	uint32_t trueTotalWidth = (totalWidth == 0 ? width : totalWidth);
	uint32_t trueTotalHeight = (totalHeight == 0 ? height : totalHeight);
    viewport.originX = x;
    viewport.originY = int(trueTotalHeight) - int(height) - y;
    viewport.width = width;
    viewport.height = height;

    scissor.x = x;
    scissor.y = int(trueTotalHeight) - int(height) - y;
    scissor.width = std::min(width, (uint32_t)(trueTotalWidth - x)) + std::min((int)scissor.x, 0);
    scissor.height = std::min(height, (uint32_t)(trueTotalHeight - scissor.y)) + std::min((int)scissor.y, 0);
    /*
    std::cout << x << ", " << y << " : " << width << ", " << height << std::endl;
    std::cout << scissor.x << ", " << scissor.y << std::endl;
    std::cout << scissor.width << ", " << scissor.height << std::endl << std::endl;
    */
	scissor.x = std::max((int)scissor.x, 0);
	scissor.y = std::max((int)scissor.y, 0);
}

void Metal_Viewport::bind() {
    g_metal_swapChain->activeRenderEncoder->setViewport(viewport);
    g_metal_swapChain->activeRenderEncoder->setScissorRect(scissor);
}

} //namespace lv
