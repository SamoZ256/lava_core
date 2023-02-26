#ifndef LV_COMMON_H
#define LV_COMMON_H

#include <string>
#include <stdexcept>

#define LV_ERROR(msg) \
throw std::runtime_error("[lv ERROR]::" + std::string(__FUNCTION__) + ": " + msg);

#endif
