#include "tek_color.hpp"

#include "../math/tek_math.hpp"

namespace tek
{
    TekColor TekColor::_white  = TekColor(255, 255, 255, 255);
    TekColor TekColor::_black  = TekColor(0, 0, 0, 255);
    TekColor TekColor::_red    = TekColor(255, 0, 0, 255);
    TekColor TekColor::_green  = TekColor(0, 255, 0, 255);
    TekColor TekColor::_blue   = TekColor(0, 0, 255, 255);
    TekColor TekColor::_yellow = TekColor(255, 255, 0, 255);

    TekColor::TekColor()
    {
	r = 255;
	g = 255;
	b = 255;
	a = 255;
    }
    
    TekColor::TekColor(u8 r, u8 g, u8 b)
    {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = 255;
    }
    
    TekColor::TekColor(u8 r, u8 g, u8 b, u8 a)
    {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
    }

    const u32 TekColor::to_int() const
    {
	return (a << 24 | b << 16 | g << 8 | r);
    }

    const Vec3 TekColor::to_vec3() const
    {
	return vec3_create(
	    r / 255.0f,
	    g / 255.0f,
	    b / 255.0f
			   );
    }

    const Vec4 TekColor::to_vec4() const
    {
	return vec4_create(
	    r / 255.0f,
	    g / 255.0f,
	    b / 255.0f,
	    a / 255.0f
      		   );
    }

    const TekColor TekColor::lighten(u8 shade) const
    {
//TODO: prevent overflow
	TekColor res;
	res.r = r + shade;
	res.g = g + shade;
	res.b = b + shade;
	res.a = a;
	return res;
    }

    const TekColor TekColor::darken(u8 shade) const
    {
//TODO: prevent underflow
	TekColor res;
	res.r = r - shade;
	res.g = g - shade;
	res.b = b - shade;
	res.a = a;
	return res;
    }

    const TekColor TekColor::lerp(TekColor c2, float t) const
    {
	float rt = (float) r + t * (c2.r - r);
	float gt = (float) g + t * (c2.g - g);
	float bt = (float) b + t * (c2.b - b);
	float at = (float) a + t * (c2.a - a);

	return TekColor((u8) rt, (u8) gt, (u8) bt, (u8) at);
    }

    const u32 TekColor::floats_to_int(float r, float g, float b, float a)
    {
	u8 ri = (u8) (r * 255);
	u8 gi = (u8) (g * 255);
	u8 bi = (u8) (b * 255);
	u8 ai = (u8) (a * 255);

	return (ai << 24 | bi << 16 | gi << 8 | ri);
    }

    const u32 TekColor::vec4_to_int(Vec4 color)
    {
	u8 ri = (u8) (color.x * 255);
	u8 gi = (u8) (color.y * 255);
	u8 bi = (u8) (color.z * 255);
	u8 ai = (u8) (color.w * 255);

	return (ai << 24 | bi << 16 | gi << 8 | ri);
    }

    const u32 TekColor::vec3_to_int(Vec3 color)
    {
	u8 ri = (u8) (color.x * 255);
	u8 gi = (u8) (color.y * 255);
	u8 bi = (u8) (color.z * 255);
	u8 ai = 255;

	return (ai << 24 | bi << 16 | gi << 8 | ri);
    }

    const TekColor TekColor::white()
    {
	return _white;
    }

    const TekColor TekColor::black()
    {
	return _black;
    }

    const TekColor TekColor::red()
    {
	return _red;
    }

    const TekColor TekColor::green()
    {
	return _green;
    }

    const TekColor TekColor::blue()
    {
	return _blue;
    }

    const TekColor TekColor::yellow()
    {
	return _yellow;
    }

    const TekColor TekColor::random()
    {
	u8 r = (u8)math_get_random_int(0,255);
	u8 g = (u8)math_get_random_int(0,255);
	u8 b = (u8)math_get_random_int(0,255);
	return TekColor(r,g,b);
    }
}
