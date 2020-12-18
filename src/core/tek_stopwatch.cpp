#include "tek_stopwatch.hpp"

#include "../platform/tek_platform.hpp"


void tek_stopwatch_start(TekStopwatch* timer)
{
	timer->started_time = tek_time_get_seconds();
	timer->paused_time = 0;
	timer->is_started = true;
	timer->is_paused = false;
}

void tek_stopwatch_stop(TekStopwatch* timer)
{
	timer->started_time = 0;
	timer->paused_time = 0;
	timer->is_started = false;
	timer->is_paused = false;
}

void tek_stopwatch_pause(TekStopwatch* timer)
{
	if (timer->is_started && !timer->is_paused)
	{
		timer->is_paused = true;
		timer->paused_time = tek_time_get_seconds();
		timer->started_time = 0;
	}
}

void tek_stopwatch_unpause(TekStopwatch* timer)
{
	if (timer->is_started && timer->is_paused)
	{
		timer->is_paused = false;
		timer->paused_time = 0;
		timer->started_time = tek_time_get_seconds();
	}
}

double tek_stopwatch_get_seconds(TekStopwatch* timer)
{
	double res = 0;
	if (timer->is_started)
	{
		if (timer->is_paused)
		{
			res = timer->paused_time;
		}
		else
		{
			res = tek_time_get_seconds() - timer->started_time;
		}
	}
	return res;
}
