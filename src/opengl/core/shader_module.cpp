#include "opengl/lvcore/core/shader_module.hpp"

#include <iostream>

#include "lvcore/common.hpp"

namespace lv {

void OpenGL_ShaderModule::init(OpenGL_ShaderModuleCreateInfo& aCreateInfo) {
    createInfo = aCreateInfo;
    shaderBundle = createInfo.shaderBundle;

    compile();
}

void OpenGL_ShaderModule::destroy() {
    glDeleteShader(shader);
}

void OpenGL_ShaderModule::compile() {
    if (createInfo.shaderStage & LV_SHADER_STAGE_VERTEX_BIT)
        shader = glCreateShader(GL_VERTEX_SHADER);
    else if (createInfo.shaderStage & LV_SHADER_STAGE_FRAGMENT_BIT)
        shader = glCreateShader(GL_FRAGMENT_SHADER);

    const char* cStrSource = createInfo.source.c_str();
    glShaderSource(shader, 1, &cStrSource, NULL);

    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        LV_ERROR("ShaderModule::compile: failed to compile shader. Log:\n" + std::string(infoLog));
    }
}

void OpenGL_ShaderModule::recompile() {
    compile();
}

} //namespace lv
