#ifndef LV_OPENGL_SHADER_MODULE_H
#define LV_OPENGL_SHADER_MODULE_H

#include <string>

#include "enums.hpp"

#include "shader_bundle.hpp"

namespace lv {

struct OpenGL_SpecializationMapEntry {
    uint32_t constantID;
    uint32_t offset;
    size_t size;
    GLenum dataType;
};

struct OpenGL_ShaderModuleCreateInfo {
    OpenGL_ShaderBundle* shaderBundle;
    LvShaderStageFlags shaderStage;
    std::string source;
    std::vector<OpenGL_SpecializationMapEntry> specializationConstants;
    void* constantsData = nullptr;
    size_t constantsSize;
};

class OpenGL_ShaderModule {
public:
    GLenum shader;

    OpenGL_ShaderModuleCreateInfo createInfo;
    OpenGL_ShaderBundle* shaderBundle;

    void init(OpenGL_ShaderModuleCreateInfo& aCreateInfo);

    void destroy();

    void compile();

    void recompile();
};

} //namespace lv

#endif
