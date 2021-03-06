#ifndef _TEK_MAT4_HPP
#define _TEK_MAT4_HPP

#include "tek_vec3.hpp"
#include "tek_vec4.hpp"

typedef struct
{
    union
    {
	float m[16];
	Vec4 rows[4];
    };
}Mat4;

void mat4_init(Mat4 *m);

Mat4 mat4_transposed(Mat4 *m);

Mat4 mat4_translate(float x, float y, float z);

Mat4 mat4_rotate(float angle, float x, float y, float z);

Mat4 mat4_scale(float x, float y, float z);

Mat4 mat4_ortho(float left, float right, float top, float bottom, float z_near, float z_far);

Mat4 mat4_perspective(float fov, float ar, float z_near, float z_far);

Mat4 mat4_lookat(Vec3 eye, Vec3 target, Vec3 up);

const Mat4 mat4_mul2(Mat4 *mat1, Mat4 *mat2);

const Mat4 mat4_mul3(Mat4 *mat1, Mat4 *mat2, Mat4 *mat3);

const Vec4 mat4_mul_vec4(Mat4 *mat1, Vec4 vec);

const Vec3 mat4_mul_vec3(Mat4 *mat1, Vec3 vec);

#endif
