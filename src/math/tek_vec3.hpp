#ifndef _TEK_VEC3_HPP
#define _TEK_VEC3_HPP

typedef struct
{
    float x;
    float y;
    float z;
}Vec3;

Vec3 vec3_create(float x, float y, float z);

const float vec3_length(Vec3 v);

const Vec3 vec3_normalized(Vec3 v);

const Vec3 vec3_negate(Vec3 v);

const float vec3_dot(Vec3 v1, Vec3 v2);

const Vec3 vec3_cross(Vec3 v1, Vec3 v2);

const float vec3_distance(Vec3 v1, Vec3 v2);

const Vec3 vec3_lerp(Vec3 v1, Vec3 v2, float t);

const Vec3 vec3_add(const Vec3 v1, const Vec3 v2);

const Vec3 vec3_sub(const Vec3 v1, const Vec3 v2);

const Vec3 vec3_mul(const Vec3 v1, float num);

Vec3 math_calc_normals(Vec3 p0, Vec3 p1, Vec3 p2);

#endif
