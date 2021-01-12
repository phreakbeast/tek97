#include "tek_stopwatch.hpp"

#include "../platform/tek_platform.hpp"

Stopwatch::Stopwatch()
{
    started_time = 0;
    paused_time = 0;
    is_started = false;
    is_paused = false;
}

void Stopwatch::start()
{
    started_time = tek_time_get_seconds();
    paused_time = 0;
    is_started = true;
    is_paused = false;
}

void Stopwatch::stop()
{
    started_time = 0;
    paused_time = 0;
    is_started = false;
    is_paused = false;
}

void Stopwatch::pause()
{
    if (is_started && !is_paused)
    {
	is_paused = true;
	paused_time = tek_time_get_seconds();
	started_time = 0;
    }
}

void Stopwatch::unpause()
{
    if (is_started && is_paused)
    {
	is_paused = false;
	paused_time = 0;
	started_time = tek_time_get_seconds();
    }
}

double Stopwatch::get_seconds()
{
    double res = 0;
    if (is_started)
    {
	if (is_paused)
	{
	    res = paused_time;
	}
	else
	{
	    res = tek_time_get_seconds() - started_time;
	}
    }
    return res;
}
