#include "tek_math_functions.hpp"

#include <stdlib.h>
#include <string.h>

void math_get_circle_coord(float rad, float pos_x, float pos_y, float angle, float *x, float *y)
{
	*x = pos_x + rad * cosf(math_to_radian(angle));
	*y = pos_y + rad * sinf(math_to_radian(angle));
}

int math_get_random_int(int min, int max)
{
	return rand() % max + min;
}

float math_get_random_float(float min, float max)
{
	float random = ((float) rand()) / (float) RAND_MAX;
	float diff = max - min;
	float r = random * diff;
	return min + r;
}

float math_to_radian(float degree)
{
	return degree * M_DEG_TO_RAD;
}

float math_to_degree(float radian)
{
	return radian * M_RAD_TO_DEG;
}

float math_rsqrtf(float x)
{
	return 1.0f / sqrtf(x);
}

float math_lerp(float v1, float v2, float t)
{
	return v1 + t * (v2 - v1);
}

