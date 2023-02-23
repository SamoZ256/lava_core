#ifndef LV_METAL_SEMAPHORE_H
#define LV_METAL_SEMAPHORE_H

#include <QuartzCore/QuartzCore.hpp>

namespace lv {

class Metal_Semaphore {
public:
    uint8_t frameCount = 0;

    dispatch_semaphore_t semaphore;

    void init();

    void destroy() {}
};

} //namespace lv

#endif
