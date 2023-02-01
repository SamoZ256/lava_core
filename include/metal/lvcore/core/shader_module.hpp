#ifndef LV_METAL_SHADER_MODULE_H
#define LV_METAL_SHADER_MODULE_H

#include <string>

#include "enums.hpp"

#include "swap_chain.hpp"

namespace lv {

dispatch_data_t convertStringToDispatchData(std::string str);

struct Metal_SpecializationMapEntry {
    uint32_t constantID;
    void* data;
    MTL::DataType dataType;
};

struct Metal_ShaderModuleCreateInfo {
    std::string source;
    const char* functionName = "main0";
    std::vector<Metal_SpecializationMapEntry> specializationConstants;
};

class Metal_ShaderModule {
public:
    MTL::Library* library;
    MTL::Function* function;

    Metal_ShaderModule(Metal_ShaderModuleCreateInfo& createInfo);

    void destroy();
};

} //namespace lv

#endif
