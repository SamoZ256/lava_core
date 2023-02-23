#ifndef LV_METAL_DESCRIPTOR_SET_H
#define LV_METAL_DESCRIPTOR_SET_H

#include <map>

#include "pipeline_layout.hpp"
#include "sampler.hpp"
#include "buffer.hpp"

namespace lv {

class Metal_DescriptorSet {
public:
    uint8_t frameCount = 0;

    Metal_PipelineLayout* pipelineLayout;
    uint8_t layoutIndex;

    std::vector<std::vector<MTL::Buffer*> > buffers;
    std::vector<uint32_t> bufferBindingIndices;
    std::vector<std::vector<MTL::Texture*> > textures;
    std::vector<uint32_t> textureBindingIndices;
    std::vector<MTL::SamplerState*> samplers;
    std::vector<uint32_t> samplerBindingIndices;

    void init() {}

    void destroy() {}

    void addBinding(Metal_BufferInfo bufferInfo, uint32_t binding);

    void addBinding(Metal_ImageInfo imageInfo, uint32_t binding);

    void bind();
};

struct Metal_DescriptorPoolCreateInfo {
    uint32_t maxSets = 1024;
	std::map<LvDescriptorType, uint16_t> poolSizes = {
		{LV_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 4},
    	{LV_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4}
	};
};

class Metal_DescriptorPool {
public:
    Metal_DescriptorPool(Metal_DescriptorPoolCreateInfo& createInfo) {}

    void init() {}

    void destroy() {}
};

} //namespace lv

#endif
