#include "metal/lvcore/core/shader_bundle.hpp"

#include <json/json.h>

#include "lvcore/filesystem/filesystem.hpp"

namespace lv {

void Metal_ShaderBundle::init(const char* filename) {
    //std::cout << filename << std::endl;
    std::string source = readFile(filename);
    nlohmann::json JSON = nlohmann::json::parse(source);

    if (JSON.contains("pushConstant")) {
        pushConstantBinding = JSON["pushConstant"]["bufferBinding"];
    }

    std::string maxSetStr = JSON["maxSet"];
    int32_t maxSet = stoi(maxSetStr);

    size_t size = sizeof(uint32_t*) * (maxSet + 1);
    bufferBindings = (uint32_t**)malloc(size);
    textureBindings = (uint32_t**)malloc(size);
    samplerBindings = (uint32_t**)malloc(size);

    for (auto& descriptorSet : JSON["descriptorSets"].items()) {
        auto& jsonDescriptorSet = JSON["descriptorSets"][descriptorSet.key()];
        std::string setNumberStr = descriptorSet.key();
        uint32_t setNumber = stoi(setNumberStr);
        std::string maxBindingStr = jsonDescriptorSet["maxBinding"];
        int32_t maxBinding = stoi(maxBindingStr);

        size = sizeof(uint32_t) * (maxBinding + 1);
        bufferBindings[setNumber] = (uint32_t*)malloc(size);
        textureBindings[setNumber] = (uint32_t*)malloc(size);
        samplerBindings[setNumber] = (uint32_t*)malloc(size);

        for (auto& binding : jsonDescriptorSet["bindings"].items()) {
            auto& jsonBinding = jsonDescriptorSet["bindings"][binding.key()];
            std::string bindingNumberStr = binding.key();
            uint32_t bindingNumber = stoi(bindingNumberStr);
            //std::cout << "Binding number: " << bindingNumber << std::endl;

            std::string descriptorType = jsonBinding["descriptorType"];
            if (descriptorType == "uniformBuffer") {
                //std::cout << "Uniform buffer" << std::endl;
                bufferBindings[setNumber][bindingNumber] = jsonBinding["bufferBinding"];
            } else if (descriptorType == "combinedImageSampler") {
                //std::cout << "Combined image sampler" << std::endl;
                textureBindings[setNumber][bindingNumber] = jsonBinding["textureBinding"];
                samplerBindings[setNumber][bindingNumber] = jsonBinding["samplerBinding"];
            } else if (descriptorType == "image") {
                textureBindings[setNumber][bindingNumber] = jsonBinding["textureBinding"];
            }
        }
    }
}

void Metal_ShaderBundle::destroy() {
    for (uint32_t i = 0; i < descriptorSetCount; i++) {
        free(bufferBindings[i]);
        free(textureBindings[i]);
        free(samplerBindings[i]);
    }
    free(bufferBindings);
    free(textureBindings);
    free(samplerBindings);
}

} //namespace lv
