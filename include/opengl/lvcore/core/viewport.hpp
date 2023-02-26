#ifndef LV_OPENGL_VIEWPORT_H
#define LV_OPENGL_VIEWPORT_H

#include <stdint.h>

namespace lv {

class OpenGL_Viewport {
public:
    int32_t x, y;
    uint32_t width, height;

    OpenGL_Viewport() {}

    OpenGL_Viewport(int32_t aX, int32_t aY, uint32_t aWidth, uint32_t aHeight);

    void setViewport(int32_t aX, int32_t aY, uint32_t aWidth, uint32_t aHeight);

	void setTotalSize(uint32_t aTotalWidth, uint32_t aTotalHeight) {}

    void bind();
};

} //namespace lv

#endif
