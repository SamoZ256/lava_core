#ifndef LV_THREAD_POOL_H
#define LV_THREAD_POOL_H

#include <vector>

#include "thread.hpp"

namespace lv {

struct ThreadPoolCreateInfo {
    uint8_t maxThreadCount = 8;
};

class ThreadPool {
public:
    std::vector<uint8_t> threadIndicesQuery;

    uint8_t maxThreadCount;

    ThreadPool(ThreadPoolCreateInfo& createInfo);

    Thread getNextAvailableThread();
};

} //namespace lv

#endif
