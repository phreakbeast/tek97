#include "tek_vec2.hpp"

#include "tek_math_functions.hpp"

Vec2 vec2_create(float x, float y)
{
    Vec2 res;
    res.x = x;
    res.y = y;
    return res;
}

const float vec2_length(Vec2 v)
{
    return sqrtf(v.x * v.x + v.y * v.y);
}

const Vec2 vec2_normalized(Vec2 v)
{
    float len = vec2_length(v);
    if (len == 0)
    {
	len = 0.0000001f;
    }
    
    Vec2 res = {
	v.x / len,
	v.y / len
    };
    return res;
}

const float vec2_dot(Vec2 v1, Vec2 v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

const float vec2_distance(Vec2 v1, Vec2 v2)
{
    float a = v1.x - v2.x;
    float b = v1.y - v2.y;
    return sqrtf(a * a + b * b);
}

const Vec2 vec2_add(const Vec2 v1, const Vec2 v2)
{
    Vec2 res = {
	v1.x + v2.x,
	v1.y + v2.y
    };
    return res;
}

const Vec2 vec2_sub(const Vec2 v1, const Vec2 v2)
{
    Vec2 res = {
	v1.x - v2.x,
	v1.y - v2.y
    };
    return res;
}

const Vec2 vec2_mul(const Vec2 v, float num)
{
    Vec2 res = {
	v.x * num,
	v.y * num
    };
    return res;
}
const Vec2 math_get_point_on_circle(float rad, float angle)
{
    float x = rad * cosf(math_to_radian(angle));
    float y = rad * sinf(math_to_radian(angle));
    
    Vec2 res = {x, y};
    return res;
}

