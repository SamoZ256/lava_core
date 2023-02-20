#include "metal/lvcore/core/semaphore.hpp"

#include "metal/lvcore/core/swap_chain.hpp"

namespace lv {

void Metal_Semaphore::init() {
    if (frameCount == -1) frameCount = g_metal_swapChain->maxFramesInFlight;

    semaphore = dispatch_semaphore_create(frameCount);
}

} //namespace lv
