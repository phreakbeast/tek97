#ifndef _TEK_QUAT_HPP
#define _TEK_QUAT_HPP

#include "tek_vec3.hpp"

typedef struct
{
    float x;
    float y;
    float z;
    float w;
}Quat;

Quat quat_create(float x, float y, float z, float w);

const float quat_length(Quat q);

const Quat quat_normalized(Quat q);

const Vec3 quat_to_euler(Quat q);

const Vec3 quat_rotate(Vec3 v, Quat q);
const Quat quat_rotation(Vec3 v1, Vec3 v2);
const Quat quat_rotation_axis(float num, Vec3 v);
const Quat quat_from_euler(Vec3 euler);

const Quat quat_add(Quat q1, Quat q2);

const Quat quat_sub(Quat q1, Quat q2);

const Quat quat_mul(Quat q1, Quat q2);

const Quat quat_mul_num(Quat q, float num);


#endif
