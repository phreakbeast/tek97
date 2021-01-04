#ifndef _TEK_VEC2_HPP
#define _TEK_VEC2_HPP


typedef struct
{
	float x;
	float y;
}Vec2;

Vec2 vec2_create(float x, float y);

const float vec2_length(Vec2 v);

const Vec2 vec2_normalized(Vec2 v);

const float vec2_dot(Vec2 v1, Vec2 v2);

const float vec2_distance(Vec2 v1, Vec2 v2);

const Vec2 vec2_add(const Vec2 v1, const Vec2 v2);

const Vec2 vec2_sub(const Vec2 v1, const Vec2 v2);

const Vec2 vec2_mul(const Vec2 v, float num);

const Vec2 math_get_point_on_circle(float rad, float angle);

#endif