#ifndef LV_VIEWPORT_H
#define LV_VIEWPORT_H

#include <vulkan/vulkan.h>

namespace lv {

class Viewport {
public:
	VkViewport viewport{};
	VkRect2D scissor{};
	uint32_t totalWidth = 0;
	uint32_t totalHeight = 0;

	Viewport(int32_t x, int32_t y, uint32_t width, uint32_t height);

	void setViewport(int32_t x, int32_t y, uint32_t width, uint32_t height);

	void setTotalSize(uint32_t aTotalWidth, uint32_t aTotalHeight) { totalWidth = aTotalWidth; totalHeight = aTotalHeight; }

	void bind();
};

} //namespace lv

#endif
