#ifndef LV_Shader_bundle_H
#define LV_Shader_bundle_H

#ifdef LV_BACKEND_VULKAN

#include "vulkan/lvcore/core/shader_bundle.hpp"

namespace lv {

typedef Vulkan_ShaderBundle ShaderBundle;

} //namespace lv

#endif

#ifdef LV_BACKEND_METAL

#include "metal/lvcore/core/shader_bundle.hpp"

namespace lv {

typedef Metal_ShaderBundle ShaderBundle;

} //namespace lv

#endif

#ifdef LV_BACKEND_OPENGL

#include "opengl/lvcore/core/shader_bundle.hpp"

namespace lv {

typedef OpenGL_ShaderBundle ShaderBundle;

} //namespace lv

#endif

#endif
