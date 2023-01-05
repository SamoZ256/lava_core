#include "lvcore/core/device.hpp"

namespace lv {

Device* g_device = nullptr;

Device::Device() {
    device = MTL::CreateSystemDefaultDevice();
    commandQueue = device->newCommandQueue();

    g_device = this;
}

void Device::destroy() {
    device->release();
    commandQueue->release();
}

} //namespace lv
