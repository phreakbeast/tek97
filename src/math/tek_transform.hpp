#ifndef _TEK_TRANSFORM_HPP
#define _TEK_TRANSFORM_HPP

#include "tek_vec3.hpp"
#include "tek_mat4.hpp"

typedef struct
{
    Vec3 position;
    Vec3 rotation;
    Vec3 size;
    Mat4 matrix;
}TekTransform;

void tek_transform_init(TekTransform* transform);
void tek_transform_calc(TekTransform* transform);

#endif
