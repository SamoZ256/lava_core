#ifndef LV_METAL_SHADER_BUNDLE_H
#define LV_METAL_SHADER_BUNDLE_H

#include <vector>

#include "enums.hpp"

namespace lv {

/*
struct Metal_BundleBinding {
    LvDescriptorType descriptorType;
    uint32_t binding;
};
*/

class Metal_ShaderBundle {
public:
    uint32_t** bufferBindings;
    uint32_t** textureBindings;
    uint32_t** samplerBindings;
    uint32_t descriptorSetCount;

    uint32_t pushConstantBinding;

    void init(const char* filename);

    void destroy();
};

} //namespace lv

#endif
