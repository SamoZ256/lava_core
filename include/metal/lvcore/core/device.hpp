#ifndef LV_DEVICE_H
#define LV_DEVICE_H

#include <Metal/Metal.hpp>

namespace lv {

class Device {
public:
    MTL::Device* device;
    MTL::CommandQueue* commandQueue;

    Device();

    void destroy();
};

extern Device* g_device;

} //namespace lv

#endif
