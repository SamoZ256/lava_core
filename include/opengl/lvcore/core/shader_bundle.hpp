#ifndef LV_OPENGL_SHADER_BUNDLE_H
#define LV_OPENGL_SHADER_BUNDLE_H

#include <vector>

#include "enums.hpp"

namespace lv {

class OpenGL_ShaderBundle {
public:
    //TODO: store binding names

    void init(const char* filename);

    void destroy();
};

} //namespace lv

#endif
