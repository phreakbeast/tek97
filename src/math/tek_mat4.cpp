#include "tek_mat4.hpp"

#include "tek_math_functions.hpp"

#include <string.h>


void mat4_init(Mat4 *mat)
{
	memset(mat->m, 0, 16 * sizeof(float));
	mat->m[0 + 0 * 4] = 1.0f;
	mat->m[1 + 1 * 4] = 1.0f;
	mat->m[2 + 2 * 4] = 1.0f;
	mat->m[3 + 3 * 4] = 1.0f;
}

Mat4 mat4_transposed(Mat4 *mat)
{
	Mat4 res;
	res.m[0] = mat->m[0];
	res.m[1] = mat->m[4];
	res.m[2] = mat->m[8];
	res.m[3] = mat->m[12];
	res.m[4] = mat->m[1];
	res.m[5] = mat->m[5];
	res.m[6] = mat->m[9];
	res.m[7] = mat->m[13];
	res.m[8] = mat->m[2];
	res.m[9] = mat->m[6];
	res.m[10] = mat->m[10];
	res.m[11] = mat->m[14];
	res.m[12] = mat->m[3];
	res.m[13] = mat->m[7];
	res.m[14] = mat->m[11];
	res.m[15] = mat->m[15];
	return res;
}

Mat4 mat4_translate(float x, float y, float z)
{
	Mat4 res;
	mat4_init(&res);

	res.m[3 + 0 * 4] = x;
	res.m[3 + 1 * 4] = y;
	res.m[3 + 2 * 4] = z;
	return res;
}

Mat4 mat4_rotate(float angle, float x, float y, float z)
{
	Mat4 res;
	mat4_init(&res);

	float r = math_to_radian(angle);
	float c = cosf(r);
	float s = sinf(r);
	float omc = 1.0f - c;

	res.m[0 + 0 * 4] = x * x * omc + c;
	res.m[0 + 1 * 4] = y * x * omc + z * s;
	res.m[0 + 2 * 4] = x * z * omc - y * s;

	res.m[1 + 0 * 4] = x * y * omc - z * s;
	res.m[1 + 1 * 4] = y * y * omc + c;
	res.m[1 + 2 * 4] = y * z * omc + x * s;

	res.m[2 + 0 * 4] = x * z * omc + y * s;
	res.m[2 + 1 * 4] = y * x * omc - x * s;
	res.m[2 + 2 * 4] = z * z * omc + c;

	return res;
}


Mat4 mat4_scale(float x, float y, float z)
{
	Mat4 res;
	mat4_init(&res);

	res.m[0 + 0 * 4] = x;
	res.m[1 + 1 * 4] = y;
	res.m[2 + 2 * 4] = z;

	return res;
}

Mat4 mat4_ortho(float left, float right, float top, float bottom, float z_near, float z_far)
{
	Mat4 res;
	mat4_init(&res);

	res.m[0 + 0 * 4] = 2.0f / (right - left);

	res.m[1 + 1 * 4] = 2.0f / (top - bottom);

	res.m[2 + 2 * 4] = 2.0f / (z_near - z_far);

	res.m[3 + 0 * 4] = (left + right) / (left - right);
	res.m[3 + 1 * 4] = (bottom + top) / (bottom - top);
	res.m[3 + 2 * 4] = (z_far + z_near) / (z_far - z_near);

	return res;
}

Mat4 mat4_perspective(float fov, float ar, float z_near, float z_far)
{
	Mat4 res;
	mat4_init(&res);

	float q = 1.0f / tanf(math_to_radian(0.5f * fov));
	float a = q / ar;

	float b = (z_near + z_far) / (z_near - z_far);
	float c = (2.0f * z_near * z_far) / (z_near - z_far);

	res.m[0 + 0 * 4] = a;
	res.m[1 + 1 * 4] = q;
	res.m[2 + 2 * 4] = b;
	res.m[2 + 3 * 4] = -1.0f;
	res.m[3 + 2 * 4] = c;

	return res;
}

Mat4 mat4_lookat(Vec3 eye, Vec3 target, Vec3 up)
{
	Vec3 z_tmp;
	z_tmp.x = eye.x - target.x;
	z_tmp.y = eye.y - target.y;
	z_tmp.z = eye.z - target.z;

	Vec3 zaxis = vec3_normalized(z_tmp);

	Vec3 xaxis = vec3_cross(up, zaxis);
	xaxis = vec3_normalized(xaxis);

	Vec3 yaxis = vec3_cross(zaxis, xaxis);

	//orientation
	Mat4 orm;
	orm.m[0] = xaxis.x;
	orm.m[4] = yaxis.x;
	orm.m[8] = zaxis.x;
	orm.m[12] = 0;
	orm.m[1] = xaxis.y;
	orm.m[5] = yaxis.y;
	orm.m[9] = zaxis.y;
	orm.m[13] = 0;
	orm.m[2] = xaxis.z;
	orm.m[6] = yaxis.z;
	orm.m[10] = zaxis.z;
	orm.m[14] = 0;
	orm.m[3] = 0;
	orm.m[7] = 0;
	orm.m[11] = 0;
	orm.m[15] = 1;

	//translation
	Mat4 trm;
	trm.m[0] = 1;
	trm.m[4] = 0;
	trm.m[8] = 0;
	trm.m[12] = 0;
	trm.m[1] = 0;
	trm.m[5] = 1;
	trm.m[9] = 0;
	trm.m[13] = 0;
	trm.m[2] = 0;
	trm.m[6] = 0;
	trm.m[10] = 1;
	trm.m[14] = 0;
	trm.m[3] = -eye.x;
	trm.m[7] = -eye.y;
	trm.m[11] = -eye.z;
	trm.m[15] = 1;

	return mat4_mul2(&orm, &trm);
}

const Mat4 mat4_mul2(Mat4 *mat1, Mat4 *mat2)
{
	Mat4 res;

	float data[16];
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			float sum = 0.0f;
			for (int e = 0; e < 4; e++)
			{
				sum += mat1->m[e + row * 4] * mat2->m[col + e * 4];
			}
			data[col + row * 4] = sum;
		}
	}
	memcpy(res.m, data, 16 * sizeof(float));

	return res;
}

const Mat4 mat4_mul3(Mat4 *mat1, Mat4 *mat2, Mat4 *mat3)
{
	Mat4 res = mat4_mul2(mat1, mat2);
	res = mat4_mul2(&res, mat3);
	return res;
}

const Vec4 mat4_mul_vec4(Mat4 *mat1, Vec4 vec)
{
	Vec4 res = {
			mat1->rows[0].x * vec.x + mat1->rows[0].y * vec.y + mat1->rows[0].z * vec.z + mat1->rows[0].w * vec.w,
			mat1->rows[1].x * vec.x + mat1->rows[1].y * vec.y + mat1->rows[1].z * vec.z + mat1->rows[1].w * vec.w,
			mat1->rows[2].x * vec.x + mat1->rows[2].y * vec.y + mat1->rows[2].z * vec.z + mat1->rows[2].w * vec.w,
			mat1->rows[3].x * vec.x + mat1->rows[3].y * vec.y + mat1->rows[3].z * vec.z + mat1->rows[3].w * vec.w
	};

	return res;
}

const Vec3 mat4_mul_vec3(Mat4 *mat1, Vec3 vec)
{
	Vec4 v = {vec.x, vec.y, vec.z, 1.0f};
	Vec4 tmp = mat4_mul_vec4(mat1, v);
	Vec3 res = {tmp.x, tmp.y, tmp.z};
	return res;
}


