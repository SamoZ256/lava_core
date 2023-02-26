#ifndef LV_METAL_DESCRIPTOR_SET_H
#define LV_METAL_DESCRIPTOR_SET_H

#include <map>

#include "enums.hpp"

#include "pipeline_layout.hpp"
#include "sampler.hpp"
#include "buffer.hpp"

namespace lv {

class OpenGL_DescriptorSet {
public:
    uint8_t frameCount = 0;

    OpenGL_PipelineLayout* pipelineLayout;
    uint8_t layoutIndex;

    bool firstTimeBound = true;

    std::vector<OpenGL_BufferInfo> bufferInfos;
    std::vector<uint32_t> bufferBindingIndices;
    std::vector<OpenGL_ImageInfo> imageInfos;
    std::vector<uint32_t> imageBindingIndices;

    void init() {}

    void destroy() {}

    void addBinding(OpenGL_BufferInfo bufferInfo, uint32_t binding) {
        bufferInfos.push_back(bufferInfo);
        bufferBindingIndices.push_back(binding);
    }

    void addBinding(OpenGL_ImageInfo imageInfo, uint32_t binding) {
        imageInfos.push_back(imageInfo);
        imageBindingIndices.push_back(binding);
    }

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
