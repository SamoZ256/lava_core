#include "opengl/lvcore/core/viewport.hpp"

#include <glad/glad.h>

namespace lv {

OpenGL_Viewport::OpenGL_Viewport(int32_t aX, int32_t aY, uint32_t aWidth, uint32_t aHeight) {
    setViewport(aX, aY, aWidth, aHeight);
}

void OpenGL_Viewport::setViewport(int32_t aX, int32_t aY, uint32_t aWidth, uint32_t aHeight) {
	x = aX;
    y = aY;
    width = aWidth;
    height = aHeight;
}

void OpenGL_Viewport::bind() {
    glViewport(x, y, width, height);
}

} //namespace lv
