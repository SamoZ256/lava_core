#ifndef LV_METAL_COMMON_H
#define LV_METAL_COMMON_H

#include <stdlib.h>

#define LV_METAL_VERTEX_BUFFER_BINDING_INDEX 30

size_t roundToMultipleOf16(size_t size);

#define MSL_SIZEOF(type) roundToMultipleOf16(sizeof(type))

#endif
