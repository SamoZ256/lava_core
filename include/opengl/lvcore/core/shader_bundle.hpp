#ifndef LV_OPENGL_SHADER_BUNDLE_H
#define LV_OPENGL_SHADER_BUNDLE_H

#include <vector>
#include <string>

#include "enums.hpp"

namespace lv {

struct OpenGL_ShaderBinding {
    bool exists = false;
    std::string name;
    LvDescriptorType descriptorType;
};

class OpenGL_ShaderBundle {
public:
    OpenGL_ShaderBinding** bindings;
    uint32_t* bindingCounts;
    uint32_t descriptorSetCount;

    std::string* pushConstantBinding = nullptr;

    void init(const char* filename);

    void destroy();
};

} //namespace lv

#endif
