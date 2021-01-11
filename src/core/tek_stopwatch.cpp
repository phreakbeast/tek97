#include "tek_stopwatch.hpp"

#include "../platform/tek_platform.hpp"


void tek_stopwatch_start(TekStopwatch* stopwatch)
{
    stopwatch->started_time = tek_time_get_seconds();
    stopwatch->paused_time = 0;
    stopwatch->is_started = true;
    stopwatch->is_paused = false;
}

void tek_stopwatch_stop(TekStopwatch* stopwatch)
{
    stopwatch->started_time = 0;
    stopwatch->paused_time = 0;
    stopwatch->is_started = false;
    stopwatch->is_paused = false;
}

void tek_stopwatch_pause(TekStopwatch* stopwatch)
{
    if (stopwatch->is_started && !stopwatch->is_paused)
    {
	stopwatch->is_paused = true;
	stopwatch->paused_time = tek_time_get_seconds();
	stopwatch->started_time = 0;
    }
}

void tek_stopwatch_unpause(TekStopwatch* stopwatch)
{
    if (stopwatch->is_started && stopwatch->is_paused)
    {
	stopwatch->is_paused = false;
	stopwatch->paused_time = 0;
	stopwatch->started_time = tek_time_get_seconds();
    }
}

double tek_stopwatch_get_seconds(TekStopwatch* stopwatch)
{
    double res = 0;
    if (stopwatch->is_started)
    {
	if (stopwatch->is_paused)
	{
	    res = stopwatch->paused_time;
	}
	else
	{
	    res = tek_time_get_seconds() - stopwatch->started_time;
	}
    }
    return res;
}
