#ifndef LV_COMMON_H
#define LV_COMMON_H

#include <stdlib.h>

size_t roundToMultipleOf16(size_t size);

#define MSL_SIZEOF(type) roundToMultipleOf16(sizeof(type))

#endif
