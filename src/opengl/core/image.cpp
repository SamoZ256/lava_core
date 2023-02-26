#include "opengl/lvcore/core/image.hpp"

#include <iostream>
#include <string>

#include <stb/stb_image.h>

#include "lvcore/common.hpp"

#include "opengl/lvcore/core/common.hpp"

#include "opengl/lvcore/core/buffer.hpp"

namespace lv {

void OpenGL_Image::init(uint16_t aWidth, uint16_t aHeight) {
    width = aWidth;
    height = aHeight;

    glGenTextures(1, &image);
    glBindTexture(viewType, image);

    getOpenGLInternalFormatProperties(format, &realFormat, &type);

    if (layerCount == 1) {
        glTexImage2D(viewType, 0, format, width, height, 0, realFormat, type, nullptr);
    } else if (viewType == LV_IMAGE_VIEW_TYPE_CUBE) {
        for (uint8_t i = 0; i < 6; i++)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, realFormat, type, nullptr);
    } else if (viewType == LV_IMAGE_VIEW_TYPE_2D_ARRAY) {
        glTexImage3D(viewType, 0, format, width, height, layerCount, 0, realFormat, type, nullptr);
    } else {
        LV_ERROR("Unsupported viewType " + std::to_string(viewType));
    }

    //Dummy values
    glTexParameteri(viewType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(viewType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(viewType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(viewType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(viewType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(viewType, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(viewType, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
    //glTexParameterf(viewType, GL_TEXTURE_MIN_LOD, 0.0f);
    //glTexParameterf(viewType, GL_TEXTURE_MAX_LOD, 0.0f);
}

void OpenGL_Image::initFromFile(const char* filename) {
    int aWidth, aHeight, nbChannels;
    void* data = (void*)stbi_load(filename, &aWidth, &aHeight, &nbChannels, STBI_rgb_alpha);
    width = aWidth;
    height = aHeight;

    if (!data) {
        throw std::runtime_error(("Failed to load image '" + std::string(filename) + "'").c_str());
    }

    init(width, height);
    copyDataTo(0, data);

    stbi_image_free(data);
}

void OpenGL_Image::copyDataTo(uint8_t threadIndex, void* data) {
    glBindTexture(viewType, image);
    if (layerCount == 1) {
        glTexSubImage2D(viewType, 0, 0, 0, width, height, realFormat, type, data);
    } else {
        throw std::runtime_error("Multilayer copies not supported right now");
    }
}

void OpenGL_Image::generateMipmaps(uint8_t threadIndex) {
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
