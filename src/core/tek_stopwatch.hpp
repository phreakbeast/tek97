#ifndef _TEK_STOPWATCH_HPP
#define _TEK_STOPWATCH_HPP

#include "tek_types.hpp"

typedef struct
{
	double started_time;
	double paused_time;
	bool is_started;
	bool is_paused;
}TekStopwatch;

void tek_stopwatch_start(TekStopwatch* timer);
void tek_stopwatch_stop(TekStopwatch* timer);
void tek_stopwatch_pause(TekStopwatch* timer);
void tek_stopwatch_unpause(TekStopwatch* timer);
double tek_stopwatch_get_seconds(TekStopwatch* timer);

#endif
