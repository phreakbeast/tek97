#ifndef _TEK_RECT_HPP
#define _TEK_RECT_HPP

#include "../core/tek_core.hpp"
#include "../math/tek_math.hpp"

namespace tek
{
    struct TekRect
    {
	float x;
	float y;
	float w;
	float h;

	TekRect();
	TekRect(float x, float y, float w, float h);
	TekRect(float x, float y, float w, float h, u32 width, u32 height);
	
	const Vec4 to_vec4() const;

	const TekRect move(float x, float y) const;

	bool is_inside(float x, float y) const;

	const TekRect create_vertical_slice(float start, float end) const;

	const TekRect create_horizontal_slice(float start, float end) const;
    };
}
#endif
