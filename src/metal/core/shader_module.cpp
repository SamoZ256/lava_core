#include "metal/lvcore/core/shader_module.hpp"

#include "metal/lvcore/core/device.hpp"

namespace lv {

Metal_ShaderModule::Metal_ShaderModule(Metal_ShaderModuleCreateInfo& createInfo) {
    dispatch_data_t sourceData = convertStringToDispatchData(createInfo.source);

    NS::Error* pError = nullptr;
    library = g_metal_device->device->newLibrary(sourceData, &pError);
    //g_metal_device->device->newLibrary()
    if (!library) {
        throw std::runtime_error(pError->localizedDescription()->utf8String());
    }

    if (createInfo.specializationConstants.size() == 0) {
        function = library->newFunction(NS::String::string(createInfo.functionName, NS::UTF8StringEncoding));
    } else {
        MTL::FunctionConstantValues* constantValues = MTL::FunctionConstantValues::alloc()->init();
        for (auto& mapEntry : createInfo.specializationConstants) {
            constantValues->setConstantValue(mapEntry.data, mapEntry.dataType, mapEntry.constantID);
        }

        MTL::FunctionDescriptor* functionDesc = MTL::FunctionDescriptor::alloc()->init();
        functionDesc->setName(NS::String::string(createInfo.functionName, NS::UTF8StringEncoding));
        functionDesc->setConstantValues(constantValues);

        //function = library->newFunction(NS::String::string(createInfo.functionName, NS::StringEncoding::UTF8StringEncoding));
        function = library->newFunction(functionDesc, &pError);
    }
}

void Metal_ShaderModule::destroy() {
    library->release();
    function->release();
}

} //namespace lv
