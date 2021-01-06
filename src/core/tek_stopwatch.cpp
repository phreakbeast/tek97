#include "tek_stopwatch.hpp"

#include "../platform/tek_platform.hpp"

namespace tek
{
    TekStopwatch::TekStopwatch()
    {
	started_time = 0;
	paused_time = 0;
	is_started = false;
	is_paused = false;
    }
    
    void TekStopwatch::start()
    {
	started_time = tek_time_get_seconds();
	paused_time = 0;
	is_started = true;
	is_paused = false;
    }

    void TekStopwatch::stop()
    {
	started_time = 0;
	paused_time = 0;
	is_started = false;
	is_paused = false;
    }

    void TekStopwatch::pause()
    {
	if (is_started && !is_paused)
	{
	    is_paused = true;
	    paused_time = tek_time_get_seconds();
	    started_time = 0;
	}
    }

    void TekStopwatch::unpause()
    {
	if (is_started && is_paused)
	{
	    is_paused = false;
	    paused_time = 0;
	    started_time = tek_time_get_seconds();
	}
    }

    double TekStopwatch::get_seconds()
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
}
