#ifndef LV_METAL_SHADER_MODULE_H
#define LV_METAL_SHADER_MODULE_H

#include <string>

#include "enums.hpp"

#include "shader_bundle.hpp"

namespace lv {

dispatch_data_t convertStringToDispatchData(std::string str);

struct Metal_SpecializationMapEntry {
    uint32_t constantID;
    uint32_t offset;
    size_t size;
    MTL::DataType dataType;
};

struct Metal_ShaderModuleCreateInfo {
    Metal_ShaderBundle* shaderBundle;
    LvShaderStageFlags shaderStage;
    std::string source;
    std::vector<Metal_SpecializationMapEntry> specializationConstants;
    void* constantsData = nullptr;
    size_t constantsSize;
};

class Metal_ShaderModule {
public:
    MTL::Library* library;
    MTL::Function* function;

    Metal_ShaderModuleCreateInfo createInfo;
    Metal_ShaderBundle* shaderBundle;

    void init(Metal_ShaderModuleCreateInfo& aCreateInfo);

    void destroy();

    void compile();

    void recompile();
};

} //namespace lv

#endif
