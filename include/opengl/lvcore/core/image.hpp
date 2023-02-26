#ifndef LV_OPENGL_IMAGE_H
#define LV_OPENGL_IMAGE_H

#include <vector>

#include "enums.hpp"

namespace lv {

class OpenGL_Image {
public:
    uint8_t frameCount = 0;

    GLuint image;

    uint16_t width, height;

    LvFormat format;
    LvImageUsageFlags usage = 0;
    LvImageAspectFlags aspectMask = 0;
    LvImageViewType viewType = LV_IMAGE_VIEW_TYPE_2D;
    LvMemoryType memoryType = LV_MEMORY_TYPE_PRIVATE;
    LvMemoryAllocationCreateFlags memoryAllocationFlags = 0;
    uint16_t layerCount = 1;
    uint16_t mipCount = 1;

    void init(uint16_t aWidth, uint16_t aHeight);

    void initFromFile(const char* filename);

    void destroy() { glDeleteTextures(1, &image); }

    void copyDataTo(uint8_t threadIndex, void* data);

    void transitionLayout(uint8_t threadIndex, uint8_t imageIndex, LvImageLayout srcLayout, LvImageLayout dstLayout) {}

    void generateMipmaps(uint8_t threadIndex);

    void copyToFromImage(uint8_t threadIndex, OpenGL_Image& source);

    void blitToFromImage(uint8_t threadIndex, OpenGL_Image& source);

private:
    GLenum realFormat, type;
};

} //namespace lv

#endif
