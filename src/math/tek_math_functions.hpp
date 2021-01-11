#ifndef _TEK_MATH_FUNCTIONS_HPP
#define _TEK_MATH_FUNCTIONS_HPP

#include "../core/tek_core.hpp"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323856f
#endif

#ifndef M_PI_2
#define M_PI_2 1.5707963267948966192313216916398f
#endif

#define M_DEG_TO_RAD 0.01745329251994329547f
#define M_RAD_TO_DEG 57.29577951308232286465f

void math_get_circle_coord(float rad, float pos_x, float pos_y, float angle, float *x, float *y);

int math_get_random_int(int min, int max);

float math_get_random_float(float min, float max);

float math_to_radian(float degree);

float math_to_degree(float radian);

float math_rsqrtf(float x);

float math_lerp(float v1, float v2, float t);

#endif
