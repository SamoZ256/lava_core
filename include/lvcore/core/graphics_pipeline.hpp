#ifndef LV_GRAPHICS_PIPELINE_H
#define LV_GRAPHICS_PIPELINE_H

#ifdef LV_BACKEND_VULKAN

#include "vulkan/lvcore/core/graphics_pipeline.hpp"

namespace lv {

typedef Vulkan_GraphicsPipelineConfig GraphicsPipelineConfig;
typedef Vulkan_GraphicsPipeline GraphicsPipeline;

} //namespace lv

#endif

#ifdef LV_BACKEND_METAL

#include "metal/lvcore/core/graphics_pipeline.hpp"

namespace lv {

typedef Metal_GraphicsPipelineConfig GraphicsPipelineConfig;
typedef Metal_GraphicsPipeline GraphicsPipeline;

} //namespace lv

#endif

#ifdef LV_BACKEND_OPENGL

#include "opengl/lvcore/core/graphics_pipeline.hpp"

namespace lv {

typedef OpenGL_GraphicsPipelineConfig GraphicsPipelineConfig;
typedef OpenGL_GraphicsPipeline GraphicsPipeline;

} //namespace lv

#endif

#endif
