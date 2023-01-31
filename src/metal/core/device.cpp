#include "metal/lvcore/core/device.hpp"

namespace lv {

Metal_Device* g_metal_device = nullptr;

Metal_Device::Metal_Device(Metal_DeviceCreateInfo& createInfo) {
    device = MTL::CreateSystemDefaultDevice();
    commandQueue = device->newCommandQueue();

    g_metal_device = this;
}

void Metal_Device::destroy() {
    device->release();
    commandQueue->release();
}

} //namespace lv
