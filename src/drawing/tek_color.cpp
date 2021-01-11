#include "tek_color.hpp"

#include "../math/tek_math.hpp"

static TekColor g_white = {255, 255, 255, 255};
static TekColor g_black = {0, 0, 0, 255};
static TekColor g_red = {255, 0, 0, 255};
static TekColor g_green = {0, 255, 0, 255};
static TekColor g_blue = {0, 0, 255, 255};
static TekColor g_yellow = {255, 255, 0, 255};

TekColor tek_color_create(u8 r, u8 g, u8 b)
{
    TekColor color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = 255;
    return color;
}

TekColor tek_color_create_alpha(u8 r, u8 g, u8 b, u8 a)
{
    TekColor color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
    return color;
}

const u32 tek_color_to_int(TekColor color)
{
    return (color.a << 24 | color.b << 16 | color.g << 8 | color.r);
}

const Vec3 tek_color_to_vec3(TekColor color)
{
    return vec3_create(
	color.r / 255.0f,
	color.g / 255.0f,
	color.b / 255.0f);
}

const Vec4 tek_color_to_vec4(TekColor color)
{
    return vec4_create(
	color.r / 255.0f,
	color.g / 255.0f,
	color.b / 255.0f,
	color.a / 255.0f);
}

const TekColor tek_color_lighten(TekColor color, u8 shade)
{
//TODO: prevent overflow
    TekColor res;
    res.r = color.r + shade;
    res.g = color.g + shade;
    res.b = color.b + shade;
    res.a = color.a;
    return res;
}

const TekColor tek_color_darken(TekColor color, u8 shade)
{
//TODO: prevent underflow
    TekColor res;
    res.r = color.r - shade;
    res.g = color.g - shade;
    res.b = color.b - shade;
    res.a = color.a;
    return res;
}

const TekColor tek_color_lerp(TekColor c1, TekColor c2, float t)
{
    float rt = (float) c1.r + t * (c2.r - c1.r);
    float gt = (float) c1.g + t * (c2.g - c1.g);
    float bt = (float) c1.b + t * (c2.b - c1.b);
    float at = (float) c1.a + t * (c2.a - c1.a);
    
    return tek_color_create_alpha((u8) rt, (u8) gt, (u8) bt, (u8) at);
}

const u32 tek_color_floats_to_int(float r, float g, float b, float a)
{
    u8 ri = (u8) (r * 255);
    u8 gi = (u8) (g * 255);
    u8 bi = (u8) (b * 255);
    u8 ai = (u8) (a * 255);
    
    return (ai << 24 | bi << 16 | gi << 8 | ri);
}

const u32 tek_color_vec4_to_int(Vec4 color)
{
    u8 ri = (u8) (color.x * 255);
    u8 gi = (u8) (color.y * 255);
    u8 bi = (u8) (color.z * 255);
    u8 ai = (u8) (color.w * 255);
    
    return (ai << 24 | bi << 16 | gi << 8 | ri);
}

const u32 tek_color_vec3_to_int(Vec3 color)
{
    u8 ri = (u8) (color.x * 255);
    u8 gi = (u8) (color.y * 255);
    u8 bi = (u8) (color.z * 255);
    u8 ai = 255;
    
    return (ai << 24 | bi << 16 | gi << 8 | ri);
}

const TekColor tek_color_white()
{
    return g_white;
}

const TekColor tek_color_black()
{
    return g_black;
}

const TekColor tek_color_red()
{
    return g_red;
}

const TekColor tek_color_green()
{
    return g_green;
}

const TekColor tek_color_blue()
{
    return g_blue;
}

const TekColor tek_color_yellow()
{
    return g_yellow;
}

const TekColor tek_color_random()
{
    u8 r = (u8) math_get_random_int(0, 255);
    u8 g = (u8) math_get_random_int(0, 255);
    u8 b = (u8) math_get_random_int(0, 255);
    return tek_color_create(r, g, b);
}

