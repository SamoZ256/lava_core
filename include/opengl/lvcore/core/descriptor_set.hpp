#ifndef LV_METAL_DESCRIPTOR_SET_H
#define LV_METAL_DESCRIPTOR_SET_H

#include <map>

#include "enums.hpp"

#include "pipeline_layout.hpp"
#include "sampler.hpp"
#include "uniform_buffer.hpp"

namespace lv {

class OpenGL_DescriptorSet {
public:
    uint8_t frameCount = 0;

    //TODO: uncomment
    //OpenGL_PipelineLayout& pipelineLayout;
    uint8_t layoutIndex;

    std::vector<std::vector<GLuint> > buffers;
    std::vector<uint32_t> bufferBindingIndices;
    std::vector<std::vector<GLuint> > textures;
    std::vector<uint32_t> textureBindingIndices;
    std::vector<GLuint> samplers;
    std::vector<uint32_t> samplerBindingIndices;

    //TODO: uncomment
    //OpenGL_DescriptorSet(OpenGL_PipelineLayout& aPipelineLayout, uint8_t aLayoutIndex) : pipelineLayout(aPipelineLayout), layoutIndex(aLayoutIndex) {}

    void init() {}

    void destroy() {}

    //TODO: uncomment
    //void addBinding(OpenGL_BufferInfo bufferInfo, uint32_t binding);

    //TODO: uncomment
    //void addBinding(OpenGL_ImageInfo imageInfo, uint32_t binding);

    void bind();
};

struct OpenGL_DescriptorPoolCreateInfo {
    uint32_t maxSets = 1024;
	std::map<LvDescriptorType, uint16_t> poolSizes = {
		{LV_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 4},
    	{LV_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4}
	};
};

class OpenGL_DescriptorPool {
public:
    OpenGL_DescriptorPool(OpenGL_DescriptorPoolCreateInfo& createInfo) {}

    void init() {}

    void destroy() {}
};

} //namespace lv

#endif
