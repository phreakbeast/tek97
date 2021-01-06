#ifndef _TEK_MEMORY_HPP
#define _TEK_MEMORY_HPP

#include "tek_types.hpp"

void* tek_malloc(u64 size);
void tek_free(void* ptr);
void* tek_realloc(void* ptr, u64 size);

#endif
