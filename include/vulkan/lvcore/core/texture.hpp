#ifndef LV_TEXTURE_H
#define LV_TEXTURE_H

#include <cassert>
#include <stdexcept>
#include <string>
#include <vector>

//#include "ImageBuffer.hpp"
#include "sampler.hpp"

namespace lv {

/*
struct TextureData {
    void* data;
    uint16_t width;
    uint16_t height;
};
*/

class Texture {
public:
    Image image;
    //VmaAllocation allocation;
    ImageView imageView;
    Sampler sampler;

    //VkFormat format = VK_FORMAT_R8G8B8A8_SRGB;
    VkFilter filter = VK_FILTER_LINEAR;

    std::string filename;

    uint16_t width, height;
    void* textureData;

    void init(/*const char* aFilename, TextureData* aTextureData = nullptr*//*, VkFormat aFormat = VK_FORMAT_R8G8B8A8_SRGB*/);

    void load(const char* aFilename);

    void destroy();

    /*
    ImageInfo descriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) {
        ImageInfo info;
        info.infos.resize(1);
        //for (uint8_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        info.infos[0].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        info.infos[0].imageView = imageView;
        info.infos[0].sampler = sampler;
        //}

        return info;
    }
    */

private:
    bool loaded = false;
};

} //namespace lv

#endif
