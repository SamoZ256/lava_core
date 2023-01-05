#ifndef LV_COMMON_H
#define LV_COMMON_H

#include <string>
#include <cassert>

#include <vulkan/vulkan.h>

#define MAX_FRAMES_IN_FLIGHT 2

std::string errorString(VkResult errorCode);

#define VK_CHECK_RESULT(f)																				\
{																										\
	VkResult res = (f);																					\
	if (res != VK_SUCCESS)																				\
	{																									\
		std::cout << "Fatal : VkResult is \"" << errorString(res) << "\" in " << __FILE__ << " at line " << __LINE__ << "\n"; \
		assert(res == VK_SUCCESS);																		\
	}																									\
}

//namespace lv {

//const std::string components[4] = {"Model", "Transform", "Material", "Script"};
/*
const std::string TRANSFORM_COMPONENT = "Transform";
const std::string MODEL_COMPONENT = "Model";
const std::string MATERIAL_COMPONENT = "Material";
const std::string SCRIPT_COMPONENT = "Script";

std::string getRelativeToAssetsPath(std::string path);
*/

/*
struct RenderPassInfo {
    VkRenderPass* renderPass;
    std::vector<VkCommandBuffer>* commandBuffers;
};

class RendInfo {
public:
    static RenderPassInfo g_renderPassInfo;
};

class Path {
public:
    static std::string assetPath;
};
*/

//} //namespace lv

#endif
