#ifndef _TEK_NOISE_HPP
#define _TEK_NOISE_HPP

#include "../core/tek_core.hpp"

	typedef struct
	{
		u32 width;
		u32 height;
		float *values;
		u32 num_values;
	}TekNoise;

void tek_noise_init(TekNoise* noise, u32 width, u32 height);

void tek_noise_destroy(TekNoise* noise);

const float tek_noise_get_value(TekNoise* noise, u32 x, u32 y);

#endif