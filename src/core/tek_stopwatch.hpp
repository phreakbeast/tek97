#ifndef _TEK_STOPWATCH_HPP
#define _TEK_STOPWATCH_HPP

#include "tek_types.hpp"

namespace tek
{
    struct TekStopwatch
    {
    public:
	TekStopwatch();
	void start();
	void stop();
	void pause();
	void unpause();
	double get_seconds();
    private:
	double started_time;
	double paused_time;
	bool is_started;
	bool is_paused;
    };
}

#endif
