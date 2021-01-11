#include "tek_vec4.hpp"

#include "tek_math_functions.hpp"

Vec4 vec4_create(float x, float y, float z, float w)
{
    Vec4 res;
    res.x = x;
    res.y = y;
    res.z = z;
    res.w = w;
    return res;
}

const float vec4_length(Vec4 v)
{
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w + v.w);
}

const Vec4 vec4_normalized(Vec4 v)
{
    float len = vec4_length(v);
    if (len == 0)
    {
	len = 0.0000001f;
    }

    Vec4 res = {
	v.x / len,
	v.y / len,
	v.z / len,
	v.w / len
    };
    return res;
}

const Vec4 vec4_lerp(Vec4 v1, Vec4 v2, float t)
{
    Vec4 res = {
	v1.x + t * (v2.x - v1.x),
	v1.y + t * (v2.y - v1.y),
	v1.z + t * (v2.z - v1.z),
	v1.w + t * (v2.w - v1.w)
    };
    return res;
}

const Vec4 vec4_add(const Vec4 v1, const Vec4 v2)
{
    Vec4 res = {
	v1.x + v2.x,
	v1.y + v2.y,
	v1.z + v2.z,
	v1.w + v2.w
    };
    return res;
}

const Vec4 vec4_sub(const Vec4 v1, const Vec4 v2)
{
    Vec4 res = {
	v1.x - v2.x,
	v1.y - v2.y,
	v1.z - v2.z,
	v1.w - v2.w
    };
    return res;
}

const Vec4 vec4_mul(const Vec4 v1, float num)
{
    Vec4 res = {
	v1.x * num,
	v1.y * num,
	v1.z * num,
	v1.w * num
    };
    return res;
}
