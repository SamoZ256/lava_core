#include "lvcore/threading/thread_pool.hpp"

namespace lv {

ThreadPool::ThreadPool(ThreadPoolCreateInfo& createInfo) {
    maxThreadCount = createInfo.maxThreadCount;
    threadIndicesQuery.resize(maxThreadCount);
    for (uint8_t i = 1; i < maxThreadCount; i++) {
        threadIndicesQuery[i] = i;
    }
}

Thread ThreadPool::getNextAvailableThread() {
    if (threadIndicesQuery.size() > 0) {
        uint8_t threadIndex = threadIndicesQuery[0];
        threadIndicesQuery.erase(threadIndicesQuery.begin());

        return Thread(threadIndex, true);
    } else {
        return Thread(0, false);
    }
}

} //namespace lv
