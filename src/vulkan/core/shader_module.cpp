#include "vulkan/lvcore/core/shader_module.hpp"

namespace lv {

void Vulkan_ShaderModule::init(Vulkan_ShaderModuleCreateInfo& aCreateInfo) {
    createInfo = aCreateInfo;
    shaderBundle = createInfo.shaderBundle;

    compile();
}

void Vulkan_ShaderModule::destroy() {
    vkDestroyShaderModule(g_vulkan_device->device(), ID, nullptr);
}

void Vulkan_ShaderModule::compile() {
    VkShaderModuleCreateInfo moduleCreateInfo{};
    moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleCreateInfo.codeSize = createInfo.source.size() * sizeof(char);
    moduleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(createInfo.source.data());

    VK_CHECK_RESULT(vkCreateShaderModule(g_vulkan_device->device(), &moduleCreateInfo, nullptr, &ID));

    //Specialization constants
    specializationInfo.mapEntryCount = createInfo.specializationConstants.size();
    specializationInfo.pMapEntries = createInfo.specializationConstants.data();
    specializationInfo.dataSize = createInfo.constantsSize;
    specializationInfo.pData = createInfo.constantsData;
    //std::cout << "Size: " << constants.size() << " : " << (constants.size() == 0 ? "none" : std::to_string((int)constants[0].constantID) + ", " + std::to_string(*(float*)constantsData) + ", " + std::to_string(constants[0].size)) << std::endl;

    stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stageInfo.stage = createInfo.shaderStage;
    stageInfo.module = ID;
    stageInfo.pName = "main";
    stageInfo.flags = 0;
    stageInfo.pNext = nullptr;
    stageInfo.pSpecializationInfo = (createInfo.specializationConstants.size() == 0 ? nullptr : &specializationInfo);
}

void Vulkan_ShaderModule::recompile() {
    destroy();
    compile();
}

} //namespace lv
