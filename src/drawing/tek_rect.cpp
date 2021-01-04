#include "tek_rect.hpp"

TekRect tek_rect_create(float x, float y, float w, float h)
{
	TekRect res;
	res.x = x;
	res.y = y;
	res.w = w;
	res.h = h;
	return res;
}

TekRect tek_rect_uv(float x, float y, float w, float h, u32 width, u32 height)
{
	float l = x / (float) width;
	float r = l + w / (float) width;
	float t = (height - y) / (float) height;
	float b = t - h / (float) height;

	TekRect res;
	res.x = l;
	res.y = b;
	res.w = r;
	res.h = t;
	return res;
}

const Vec4 tek_rect_to_vec4(TekRect rect)
{
	return vec4_create(rect.x, rect.y, rect.w, rect.h);
}

const TekRect tek_rect_move(TekRect rect, float x, float y)
{
	TekRect res;
	res.x = rect.x + x;
	res.y = rect.y + y;
	res.w = rect.w;
	res.h = rect.h;
	return res;
}

bool tek_rect_is_inside(TekRect rect, float x, float y)
{
	float min_x = rect.x;
	float max_x = rect.x + rect.w;
	float min_y = rect.y;
	float max_y = rect.y + rect.h;

	if (x > min_x && x <= max_x &&
	    y > min_y && y <= max_y)
	{
		return true;
	}

	return false;
}

const TekRect tek_rect_create_vertical_slice(TekRect rect, float start, float end)
{
	float res_y = rect.y + rect.h * start;
	float res_h = rect.h * end - rect.h * start;
	TekRect res = {rect.x, res_y, rect.w, res_h};
	return res;
}

const TekRect tek_rect_horizontal_slice(TekRect rect, float start, float end)
{
	float res_x = rect.x + rect.w * start;
	float res_w = rect.w * end - rect.w * start;
	TekRect res = {res_x, rect.y, res_w, rect.h};
	return res;
}

