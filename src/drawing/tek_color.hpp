#ifndef _TEK_COLOR_HPP
#define _TEK_COLOR_HPP

#include "../core/tek_core.hpp"
#include "../math/tek_math.hpp"

struct TekColor
{
	u8 r;
	u8 g;
	u8 b;
	u8 a;
};

TekColor tek_color_create(u8 r, u8 g, u8 b);
TekColor tek_color_create_alpha(u8 r, u8 g, u8 b, u8 a);

const u32 tek_color_to_int(TekColor col);

const Vec3 tek_color_to_vec3(TekColor col);

const Vec4 tek_color_to_vec4(TekColor col);

const TekColor tek_color_lighten(TekColor col, u8 shade);

const TekColor tek_color_darken(TekColor col, u8 shade);

const TekColor tek_color_lerp(TekColor c1, TekColor c2, float t);

const u32 tek_color_floats_to_int(float r, float g, float b, float a);

const u32 tek_color_vec4_to_int(Vec4 color);

const u32 tek_color_vec3_to_int(Vec3 color);

const TekColor tek_color_white();
const TekColor tek_color_black();
const TekColor tek_color_red();
const TekColor tek_color_green();
const TekColor tek_color_blue();
const TekColor tek_color_yellow();
const TekColor tek_color_random();

#endif
