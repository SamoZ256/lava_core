#include "lvcore/core/buffer.hpp"

#include "lvcore/core/device.hpp"

namespace lv {

void Buffer::init(void* data, size_t aSize) {
    size = aSize;
    buffer = g_device->device->newBuffer(size, MTL::ResourceStorageModeManaged);
    memcpy(buffer->contents(), data, size);

    buffer->didModifyRange(NS::Range::Make(0, buffer->length()));
}

void Buffer::destroy() {
    buffer->release();
}

} //namespace lv
