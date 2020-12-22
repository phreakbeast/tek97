#ifndef _TEK_NOISE_HPP
#define _TEK_NOISE_HPP

#include "../core/tek_core.hpp"

struct TekNoise
{
	TekNoise(u32 width, u32 height);
	~TekNoise();

	const float get_value(u32 x, u32 y);
	const u32 get_width() const {return width;}
	const u32 get_height() const {return height;}
private:
	double smooth_noise(double x, double y);
	double turbulence(double x, double y, double size);

	u32 width;
	u32 height;
	float *values;
	u32 num_values;
};

#endif