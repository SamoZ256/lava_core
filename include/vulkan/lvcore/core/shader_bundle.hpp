#ifndef LV_VULKAN_SHADER_BUNDLE_H
#define LV_VULKAN_SHADER_BUNDLE_H

#include <vector>

#include "enums.hpp"

namespace lv {

class Vulkan_ShaderBundle {
public:
    void init(const char* filename);

    void destroy() {}
};

} //namespace lv

#endif
