#include "../tek_platform.hpp"

#include <time.h>


static struct timespec g_start_ticks;

void tek_time_init()
{
	clock_gettime(CLOCK_REALTIME, &g_start_ticks);
}

double tek_time_get_seconds()
{
	struct timespec now;
	clock_gettime(CLOCK_REALTIME, &now);
	u32 res = (now.tv_sec - g_start_ticks.tv_sec) * 1000 + (now.tv_nsec - g_start_ticks.tv_nsec) / 1000000;
	return res / 1000.0;
}
