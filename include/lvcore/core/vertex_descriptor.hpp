#ifndef LV_VERTEX_DESCRIPTOR_H
#define LV_VERTEX_DESCRIPTOR_H

#ifdef LV_BACKEND_VULKAN

#include "vulkan/lvcore/core/vertex_descriptor.hpp"

namespace lv {

typedef Vulkan_VertexDescriptor VertexDescriptor;

} //namespace lv

#endif

#ifdef LV_BACKEND_METAL

#include "metal/lvcore/core/vertex_descriptor.hpp"

namespace lv {

typedef Metal_VertexDescriptor VertexDescriptor;

} //namespace lv

#endif

#ifdef LV_BACKEND_OPENGL

#include "opengl/lvcore/core/vertex_descriptor.hpp"

namespace lv {

typedef OpenGL_VertexDescriptor VertexDescriptor;

} //namespace lv

#endif

#endif
