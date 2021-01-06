#include "tek_rect.hpp"

namespace tek
{
    TekRect::TekRect()
    {
	x = 0;
	y = 0;
	w = 0;
	h = 0;
    }
    
    TekRect::TekRect(float x, float y, float w, float h)
    {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
    }

    TekRect::TekRect(float x, float y, float w, float h, u32 width, u32 height)
    {
	float l = x / (float) width;
	float r = l + w / (float) width;
	float t = (height - y) / (float) height;
	float b = t - h / (float) height;

	this->x = l;
	this->y = b;
	this->w = r;
	this->h = t;
    }

    const Vec4 TekRect::to_vec4() const
    {
	return vec4_create(x, y, w, h);
    }

    const TekRect TekRect::move(float x, float y) const
    {
	TekRect res;
	res.x = this->x + x;
	res.y = this->y + y;
	res.w = this->w;
	res.h = this->h;
	return res;
    }

    bool TekRect::is_inside(float x, float y) const
    {
	float min_x = this->x;
	float max_x = this->x + this->w;
	float min_y = this->y;
	float max_y = this->y + this->h;

	if (x > min_x && x <= max_x &&
	    y > min_y && y <= max_y)
	{
	    return true;
	}

	return false;
    }

    const TekRect TekRect::create_vertical_slice(float start, float end) const
    {
	float res_y = y + h * start;
	float res_h = h * end - h * start;
	TekRect res = {x, res_y, w, res_h};
	return res;
    }

    const TekRect TekRect::create_horizontal_slice(float start, float end) const
    {
	float res_x = x + w * start;
	float res_w = w * end - w * start;
	TekRect res = {res_x, y, res_w, h};
	return res;
    }

}
