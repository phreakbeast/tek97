#include "tek_color.hpp"

#include "../math/tek_math.hpp"

Color::Color()
{
    r = 255;
    g = 255;
    b = 255;
    a = 255;
}

Color::Color(u8 r, u8 g, u8 b)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = 255;
}

Color::Color(u8 r, u8 g, u8 b, u8 a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

const u32 Color::to_int() const
{
    return (a << 24 | b << 16 | g << 8 | r);
}

const Vec3 Color::to_vec3() const
{
    return vec3_create(
	r / 255.0f,
	g / 255.0f,
	b / 255.0f);
}

const Vec4 Color::to_vec4() const
{
    return vec4_create(
	r / 255.0f,
	g / 255.0f,
	b / 255.0f,
	a / 255.0f);
}

const Color Color::lighten(u8 shade) const
{
//TODO: prevent overflow
    Color res;
    res.r = r + shade;
    res.g = g + shade;
    res.b = b + shade;
    res.a = a;
    return res;
}

const Color Color::darken(u8 shade) const
{
//TODO: prevent underflow
    Color res;
    res.r = r - shade;
    res.g = g - shade;
    res.b = b - shade;
    res.a = a;
    return res;
}

const Color Color::lerp(Color c2, float t) const
{
    float rt = (float) r + t * (c2.r - r);
    float gt = (float) g + t * (c2.g - g);
    float bt = (float) b + t * (c2.b - b);
    float at = (float) a + t * (c2.a - a);
    
    return Color((u8) rt, (u8) gt, (u8) bt, (u8) at);
}

const u32 Color::floats_to_int(float r, float g, float b, float a)
{
    u8 ri = (u8) (r * 255);
    u8 gi = (u8) (g * 255);
    u8 bi = (u8) (b * 255);
    u8 ai = (u8) (a * 255);
    
    return (ai << 24 | bi << 16 | gi << 8 | ri);
}

const u32 Color::vec4_to_int(Vec4 color)
{
    u8 ri = (u8) (color.x * 255);
    u8 gi = (u8) (color.y * 255);
    u8 bi = (u8) (color.z * 255);
    u8 ai = (u8) (color.w * 255);
    
    return (ai << 24 | bi << 16 | gi << 8 | ri);
}

const u32 Color::vec3_to_int(Vec3 color)
{
    u8 ri = (u8) (color.x * 255);
    u8 gi = (u8) (color.y * 255);
    u8 bi = (u8) (color.z * 255);
    u8 ai = 255;
    
    return (ai << 24 | bi << 16 | gi << 8 | ri);
}

const Color Color::white()
{
    return Color(255,255,255);
}

const Color Color::black()
{
    return Color(0,0,0);
}

const Color Color::red()
{
    return Color(255,0,0);
}

const Color Color::green()
{
    return Color(0,255,0);
}

const Color Color::blue()
{
    return Color(0,0,255);
}

const Color Color::yellow()
{
    return Color(0,255,255);
}

const Color Color::random()
{
    u8 r = (u8) math_get_random_int(0, 255);
    u8 g = (u8) math_get_random_int(0, 255);
    u8 b = (u8) math_get_random_int(0, 255);
    return Color(r, g, b);
}

