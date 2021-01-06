#ifndef _TEK_TRANSFORM_HPP
#define _TEK_TRANSFORM_HPP

#include "tek_math.hpp"

typedef struct
{
	Vec3 position;
	Vec3 rotation;
	Vec3 size;
	Mat4 matrix;
}TekTransform;


TekTransform tek_transform_create(Vec3 position, Vec3 rotation, Vec3 size);

void
tek_transform_init(TekTransform* transform, float x, float y, float z, float rx, float ry, float rz, float sx, float sy,
	float sz);

void tek_transform_calc(TekTransform* transform);

#endif
