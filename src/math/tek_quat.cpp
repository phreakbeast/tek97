#include "tek_quat.hpp"

#include "tek_math_functions.hpp"

static inline float norm_helper(Quat q)
{
    float result;
    result = (q.x * q.x);
    result = (result + (q.y * q.y));
    result = (result + (q.z * q.z));
    result = (result + (q.w * q.w));
    return result;
}


Quat quat_create(float x, float y, float z, float w)
{
    Quat res;
    res.x = x;
    res.y = y;
    res.z = z;
    res.w = w;
    return res;
}

const float quat_length(Quat q)
{
    return sqrtf(norm_helper(q));
}

const Quat quat_normalized(Quat q)
{
    float len_sqr = norm_helper(q);
    float len_inv = math_rsqrtf(len_sqr);
    return quat_mul_num(q, len_inv);
}

const Vec3 quat_to_euler(Quat q)
{
    Vec3 res = {
	atan2f(2 * q.x * q.w - 2 * q.y * q.z, 1 - 2 * q.x * q.x - 2 * q.z * q.z),
	atan2f(2 * q.y * q.w - 2 * q.x * q.z, 1 - 2 * q.y * q.y - 2 * q.z * q.z),
	asinf(2 * q.x * q.y + 2 * q.z * q.w)
    };
    return res;
}


const Vec3 quat_rotate(Vec3 v, Quat q)
{
    float tmpX, tmpY, tmpZ, tmpW;
    tmpX = (((q.w * v.x) + (q.y * v.z)) - (q.z * v.y));
    tmpY = (((q.w * v.y) + (q.z * v.x)) - (q.x * v.z));
    tmpZ = (((q.w * v.z) + (q.x * v.y)) - (q.y * v.x));
    tmpW = (((q.x * v.x) + (q.y * v.y)) + (q.z * v.z));

    Vec3 res;
    res.x = ((((tmpW * q.x) + (tmpX * q.w)) - (tmpY * q.z)) + (tmpZ * q.y));
    res.y = ((((tmpW * q.y) + (tmpY * q.w)) - (tmpZ * q.x)) + (tmpX * q.z));
    res.z = ((((tmpW * q.z) + (tmpZ * q.w)) - (tmpX * q.y)) + (tmpY * q.x));

    return res;
}

const Quat quat_rotation(Vec3 v1, Vec3 v2)
{
    float cosHalfAngleX2, recipCosHalfAngleX2;
    float dot = vec3_dot(v1, v2);
    cosHalfAngleX2 = sqrtf((2.0f * (1.0f + dot)));
    recipCosHalfAngleX2 = (1.0f / cosHalfAngleX2);
    Vec3 tmp = vec3_cross(v1, v2);
    tmp = vec3_mul(tmp, recipCosHalfAngleX2);
    Quat res;
    res.x = tmp.x;
    res.y = tmp.y;
    res.z = tmp.z;
    res.w = cosHalfAngleX2 * 0.5f;
    return res;
}

const Quat quat_rotation_axis(float num, Vec3 v)
{
    float angle = num * 0.5f;
    Vec3 t = vec3_mul(v, (sinf(angle)));
    Quat res;
    res.x = t.x;
    res.y = t.y;
    res.z = t.z;
    res.w = cosf(angle);
    return res;
}

const Quat quat_from_euler(Vec3 euler)
{
    Quat pitch;
    pitch.x = 1;
    pitch.y = 0;
    pitch.z = 0;
    pitch.w = euler.x;

    Quat yaw;
    yaw.x = 0;
    yaw.y = 1;
    yaw.z = 0;
    yaw.w = euler.y;

    Quat roll;
    roll.x = 0;
    roll.y = 0;
    roll.z = 1;
    roll.w = euler.z;

    Quat t = quat_mul(pitch, yaw);
    t = quat_mul(t, roll);
    return t;
}

const Quat quat_add(Quat q1, Quat q2)
{
    Quat res;
    res.x = q1.x + q2.x;
    res.y = q1.y + q2.y;
    res.z = q1.z + q2.z;
    res.w = q1.w + q2.w;
    return res;
}

const Quat quat_sub(Quat q1, Quat q2)
{
    Quat res;
    res.x = q1.x - q2.x;
    res.y = q1.y - q2.y;
    res.z = q1.z - q2.z;
    res.w = q1.w - q2.w;
    return res;
}

const Quat quat_mul_num(Quat q, float num)
{
    Quat res;
    res.x = q.x * num;
    res.y = q.y * num;
    res.z = q.z * num;
    res.w = q.w * num;
    return res;
}

const Quat quat_mul(Quat q1, Quat q2)
{
    Quat res;

    res.x = (((q1.w * q2.x) + (q1.x * q2.w)) + (q1.y * q2.z)) - (q1.z * q2.y);
    res.y = (((q1.w * q2.y) + (q1.y * q2.w)) + (q1.z * q2.x)) - (q1.x * q2.z);
    res.z = (((q1.w * q2.z) + (q1.z * q2.w)) + (q1.x * q2.y)) - (q1.y * q2.x);
    res.w = (((q1.w * q2.w) - (q1.x * q2.x)) - (q1.y * q2.y)) - (q1.z * q2.z);

    return quat_normalized(res);
}
