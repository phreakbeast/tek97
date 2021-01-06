#include "tek_memory.hpp"

#include <stdlib.h>

void* tek_malloc(u64 size)
{
	return malloc(size);
}

void tek_free(void* ptr)
{
	free(ptr);
}

void* tek_realloc(void* ptr, u64 size)
{
	return realloc(ptr,size);
}
