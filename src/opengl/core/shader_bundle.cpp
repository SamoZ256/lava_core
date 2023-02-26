#include "opengl/lvcore/core/shader_bundle.hpp"

#include <json/json.h>

#include "lvcore/filesystem/filesystem.hpp"

namespace lv {

void OpenGL_ShaderBundle::init(const char* filename) {
    //std::cout << filename << std::endl;
    std::string source = readFile(filename);
    nlohmann::json JSON = nlohmann::json::parse(source);

    if (JSON.contains("pushConstant")) {
        pushConstantBinding = new std::string(JSON["pushConstant"]["name"]);
    }

    std::string maxSetStr = JSON["maxSet"];
    descriptorSetCount = stoi(maxSetStr) + 1;

    bindings = (OpenGL_ShaderBinding**)malloc(sizeof(OpenGL_ShaderBinding*) * descriptorSetCount);
    bindingCounts = (uint32_t*)malloc(sizeof(uint32_t) * descriptorSetCount);

    for (auto& descriptorSet : JSON["descriptorSets"].items()) {
        auto& jsonDescriptorSet = JSON["descriptorSets"][descriptorSet.key()];
        std::string setNumberStr = descriptorSet.key();
        uint32_t setNumber = stoi(setNumberStr);
        std::string maxBindingStr = jsonDescriptorSet["maxBinding"];
        bindingCounts[setNumber] = stoi(maxBindingStr) + 1;

        bindings[setNumber] = (OpenGL_ShaderBinding*)malloc(sizeof(OpenGL_ShaderBinding) * bindingCounts[setNumber]);

        for (auto& binding : jsonDescriptorSet["bindings"].items()) {
            auto& jsonBinding = jsonDescriptorSet["bindings"][binding.key()];
            std::string bindingNumberStr = binding.key();
            uint32_t bindingNumber = stoi(bindingNumberStr);
            bindings[setNumber][bindingNumber].exists = true;
            bindings[setNumber][bindingNumber].name = jsonBinding["name"];

            std::string descriptorType = jsonBinding["descriptorType"];
            if (descriptorType == "uniformBuffer")
                bindings[setNumber][bindingNumber].descriptorType = LV_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            else if (descriptorType == "combinedImageSampler")
                bindings[setNumber][bindingNumber].descriptorType = LV_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        }
    }
}

void OpenGL_ShaderBundle::destroy() {
    for (uint32_t i = 0; i < descriptorSetCount; i++)
        free(bindings[i]);
    free(bindings);
    if (pushConstantBinding)
        delete pushConstantBinding;
}

} //namespace lv
