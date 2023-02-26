#include "opengl/lvcore/core/shader_bundle.hpp"

#include <json/json.h>

#include "lvcore/filesystem/filesystem.hpp"

namespace lv {

void OpenGL_ShaderBundle::init(const char* filename) {
    //std::cout << filename << std::endl;
    std::string source = readFile(filename);
    nlohmann::json JSON = nlohmann::json::parse(source);
    std::cout << "Shader bundle 1" << std::endl;

    if (JSON.contains("pushConstant")) {
        pushConstantBinding = new std::string(JSON["pushConstant"]["name"]);
    }
    std::cout << "Shader bundle 2" << std::endl;

    std::string maxSetStr = JSON["maxSet"];
    descriptorSetCount = stoi(maxSetStr) + 1;

    bindings = (OpenGL_ShaderBinding**)malloc(sizeof(OpenGL_ShaderBinding*) * descriptorSetCount);
    bindingCounts = (uint32_t*)malloc(sizeof(uint32_t) * descriptorSetCount);
    std::cout << "Shader bundle 3" << std::endl;

    for (auto& descriptorSet : JSON["descriptorSets"].items()) {
        auto& jsonDescriptorSet = JSON["descriptorSets"][descriptorSet.key()];
        std::string setNumberStr = descriptorSet.key();
        uint32_t setNumber = stoi(setNumberStr);
        std::string maxBindingStr = jsonDescriptorSet["maxBinding"];
        bindingCounts[setNumber] = stoi(maxBindingStr) + 1;

        bindings[setNumber] = (OpenGL_ShaderBinding*)malloc(sizeof(OpenGL_ShaderBinding) * bindingCounts[setNumber]);
        std::cout << "Allocated size: " << sizeof(OpenGL_ShaderBinding) * bindingCounts[setNumber] << std::endl;
        std::cout << "Shader bundle 4" << std::endl;

        for (auto& binding : jsonDescriptorSet["bindings"].items()) {
            std::cout << "Shader bundle 4.1" << std::endl;
            auto& jsonBinding = jsonDescriptorSet["bindings"][binding.key()];
            std::cout << "Shader bundle 4.1.1" << std::endl;
            std::string bindingNumberStr = binding.key();
            std::cout << "Shader bundle 4.1.2" << std::endl;
            uint32_t bindingNumber = stoi(bindingNumberStr);
            std::cout << "Shader bundle 4.1.3" << std::endl;
            bindings[setNumber][bindingNumber].exists = true;
            std::cout << "Shader bundle 4.1.4" << std::endl;
            std::string name = jsonBinding["name"];
            std::cout << (int)bindingCounts[setNumber] << " : " << (int)bindingNumber << std::endl;
            std::cout << name << std::endl;
            bindings[setNumber][bindingNumber].name = jsonBinding["name"];
            std::cout << "Shader bundle 4.2" << std::endl;

            std::string descriptorType = jsonBinding["descriptorType"];
            if (descriptorType == "uniformBuffer")
                bindings[setNumber][bindingNumber].descriptorType = LV_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            else if (descriptorType == "combinedImageSampler")
                bindings[setNumber][bindingNumber].descriptorType = LV_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            std::cout << "Shader bundle 4.3" << std::endl;
        }
        std::cout << "Shader bundle 5" << std::endl;
    }
    std::cout << "Shader bundle 6" << std::endl;
}

void OpenGL_ShaderBundle::destroy() {
    for (uint32_t i = 0; i < descriptorSetCount; i++)
        free(bindings[i]);
    free(bindings);
    if (pushConstantBinding)
        delete pushConstantBinding;
}

} //namespace lv
