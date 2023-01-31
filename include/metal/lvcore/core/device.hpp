#ifndef LV_METAL_DEVICE_H
#define LV_METAL_DEVICE_H

#include <Metal/Metal.hpp>

#include "lvnd/lvnd.h"

namespace lv {

struct Metal_DeviceCreateInfo {
    LvndWindow* window;
};

class Metal_Device {
public:
    MTL::Device* device;
    MTL::CommandQueue* commandQueue;

    Metal_Device(Metal_DeviceCreateInfo& createInfo);

    void destroy();
};

extern Metal_Device* g_metal_device;

} //namespace lv

#endif
