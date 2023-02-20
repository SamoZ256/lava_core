#ifndef LV_METAL_SEMAPHORE_H
#define LV_METAL_SEMAPHORE_H

#include <QuartzCore/QuartzCore.hpp>

namespace lv {

class Metal_Semaphore {
public:
    int8_t frameCount = -1;

    dispatch_semaphore_t semaphore;

    void init();

    void destroy() {}
};

} //namespace lv

#endif
