#include "metal/lvcore/core/shader_module.hpp"

#include "metal/lvcore/core/device.hpp"
#include "metal/lvcore/core/swap_chain.hpp"

namespace lv {

void Metal_ShaderModule::init(Metal_ShaderModuleCreateInfo& aCreateInfo) {
    createInfo = aCreateInfo;
    shaderBundle = createInfo.shaderBundle;

    dispatch_data_t sourceData = convertStringToDispatchData(createInfo.source);

    NS::Error* pError = nullptr;
    library = g_metal_device->device->newLibrary(sourceData, &pError);
    //g_metal_device->device->newLibrary()
    if (!library) {
        throw std::runtime_error(pError->localizedDescription()->utf8String());
    }

    compile();
}

void Metal_ShaderModule::destroy() {
    library->release();
    function->release();
}

void Metal_ShaderModule::compile() {
    NS::Error* pError = nullptr;
    if (createInfo.specializationConstants.size() == 0) {
        function = library->newFunction(NS::String::string("main0", NS::UTF8StringEncoding));
    } else {
        MTL::FunctionDescriptor* functionDesc = MTL::FunctionDescriptor::alloc()->init();
        functionDesc->setName(NS::String::string("main0", NS::UTF8StringEncoding));

        if (createInfo.specializationConstants.size() > 0) {
            MTL::FunctionConstantValues* constantValues = MTL::FunctionConstantValues::alloc()->init();
            //char* constantsData = (char*)createInfo.constantsData;
            for (auto& mapEntry : createInfo.specializationConstants) {
                constantValues->setConstantValue(/*((void**)createInfo.constantsData)[mapEntry.offset]*/(char*)createInfo.constantsData + mapEntry.offset, mapEntry.dataType, mapEntry.constantID);
            }

            functionDesc->setConstantValues(constantValues);
        }

        //function = library->newFunction(NS::String::string(createInfo.functionName, NS::StringEncoding::UTF8StringEncoding));
        function = library->newFunction(functionDesc, &pError);
    }
}

void Metal_ShaderModule::recompile() {
    function->release();
    compile();
}

} //namespace lv
