#ifndef _TEK_QUAT_HPP
#define _TEK_QUAT_HPP

#include "tek_vec3.hpp"

typedef struct
{
	float x;
	float y;
	float z;
	float w;
} Quat;

Quat quat_create(float x, float y, float z, float w);

const float quat_length(Quat q);

const Quat quat_normalize(Quat q);

const Vec3 quat_to_euler(Quat q);

Vec3 quat_rotate(Vec3 v, Quat q);

Quat quat_rotation(Vec3 v1, Vec3 v2);

Quat quat_rotation_axis(float num, Vec3 v);

Quat quat_from_euler(Vec3 euler);

const Quat quat_add(const Quat q1, const Quat q2);

const Quat quat_sub(const Quat q1, const Quat q2);

const Quat quat_mul(const Quat q1, const Quat q2);

const Quat quat_mul_num(const Quat q, float num);


#endif