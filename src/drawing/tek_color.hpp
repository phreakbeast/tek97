#ifndef _TEK_COLOR_HPP
#define _TEK_COLOR_HPP

#include "../core/tek_core.hpp"
#include "../math/tek_math.hpp"

namespace tek
{
    struct TekColor
    {
	u8 r;
	u8 g;
	u8 b;
	u8 a;

	TekColor();
	TekColor(u8 r, u8 g, u8 b);
	TekColor(u8 r, u8 g, u8 b, u8 a);

	const u32 to_int() const;

	const Vec3 to_vec3() const;

	const Vec4 to_vec4() const;
	
	const TekColor lighten(u8 shade) const;

	const TekColor darken(u8 shade) const;

	const TekColor lerp(TekColor c2, float t) const;
	
	static const u32 floats_to_int(float r, float g, float b, float a);

	static const u32 vec4_to_int(Vec4 color);
	
	static const u32 vec3_to_int(Vec3 color);

	static const TekColor white();
	static const TekColor black();
	static const TekColor red();
	static const TekColor green();
	static const TekColor blue();
	static const TekColor yellow();
	static const TekColor random();
    private:
	static TekColor _white;
	static TekColor _black;
	static TekColor _red;
	static TekColor _green;
	static TekColor _blue;
	static TekColor _yellow;
    };
}
#endif
