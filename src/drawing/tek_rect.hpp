#ifndef _TEK_RECT_HPP
#define _TEK_RECT_HPP

#include "../core/tek_core.hpp"
#include "../math/tek_math.hpp"

typedef struct
{
    float x;
    float y;
    float w;
    float h;
}TekRect;

TekRect tek_rect_create(float x, float y, float w, float h);

TekRect tek_rect_uv(float x, float y, float w, float h, u32 width, u32 height);

const Vec4 tek_rect_to_vec4(TekRect rect);

const TekRect tek_rect_move(TekRect rect, float x, float y);

bool tek_rect_is_inside(TekRect rect,float x, float y);

const TekRect tek_rect_create_vertical_slice(TekRect rect, float start, float end);

const TekRect tek_rect_create_horizontal_slice(TekRect rect, float start, float end);

#endif
