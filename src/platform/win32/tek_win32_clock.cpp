#include "../tek_platform.hpp"

#include <Windows.h>

static DWORD g_start_ticks;

void tek_time_init()
{
    g_start_ticks = GetTickCount64();
}


double tek_time_get_seconds()
{
    DWORD ticks = GetTickCount64();
    DWORD new_ticks = ticks - g_start_ticks;
    double seconds = new_ticks / 1000.0;

    return seconds;
}

void tek_time_sleep_ms(int milliseconds)
{
    Sleep(milliseconds);
}