#include "opengl/lvcore/core/image.hpp"

#include "opengl/lvcore/core/buffer.hpp"

namespace lv {

void getOpenGLInternalFormatProperties(LvFormat internalFormat, GLenum* format, GLenum* type) {
    //TODO: use switch statement to find the properties
    //TODO: move this to common.hpp file
}

void OpenGL_Image::init(uint16_t aWidth, uint16_t aHeight) {
    width = aWidth;
    height = aHeight;

    glGenTextures(1, &image);
    glBindTexture(viewType, image);

    getOpenGLInternalFormatProperties(format, &realFormat, &type);

    if (layerCount == 1) {
        glTexImage2D(viewType, 0, format, width, height, 0, realFormat, type, nullptr);
    } else if (viewType == LV_IMAGE_VIEW_TYPE_CUBE) {
        //TODO: create cube map
    } else {
        glTexImage3D(viewType, 0, format, width, height, layerCount, 0, realFormat, type, nullptr);
    }
}

void OpenGL_Image::copyDataTo(uint8_t threadIndex, void* data) {
    glBindTexture(viewType, image);
    if (layerCount == 1) {
        glTexSubImage2D(viewType, 0, 0, 0, width, height, realFormat, type, data);
    } else {
        throw std::runtime_error("Multilayer copies not supported right now");
    }
}

void OpenGL_Image::generateMipmaps() {
    glBindTexture(viewType, image);
    glGenerateMipmap(viewType);
}

void OpenGL_Image::copyToFromImage(uint8_t threadIndex, OpenGL_Image& source) {
    //TODO: implement this
}

void OpenGL_Image::blitToFromImage(uint8_t threadIndex, OpenGL_Image& source) {
    //TODO: implement this
}

} //namespace lv
