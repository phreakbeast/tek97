#include "tek_transform.hpp"

TekTransform::TekTransform()
{
	this->size = Vec3(1);
	calc();
}

TekTransform::TekTransform(Vec3 position, Vec3 rotation, Vec3 size)
{
	this->position = position;
	this->rotation = rotation;
	this->size = size;
	calc();
}

TekTransform::TekTransform(float x, float y, float z, float rx, float ry, float rz, float sx, float sy, float sz)
{
	position = vec3_create(x, y, z);
	rotation = vec3_create(rx, ry, rz);
	size = vec3_create(sx, sy, sz);
	calc();
}

void TekTransform::calc()
{
	Mat4 trans = mat4_translate(position.x, position.y, position.z);
	Mat4 rot_x = mat4_rotate(rotation.x, 1, 0, 0);
	Mat4 rot_y = mat4_rotate(rotation.y, 0, 1, 0);
	Mat4 rot_z = mat4_rotate(rotation.z, 0, 0, 1);
	Mat4 rot = mat4_mul3(&rot_y, &rot_x, &rot_z);
	Mat4 scale = mat4_scale(size.x, size.y, size.z);

	matrix = mat4_mul3(&trans, &rot, &scale);
}

TekTransform tek_transform_create(Vec3 position, Vec3 rotation, Vec3 size)
{
    TekTransform res;
    res.position = position;
    res.rotation = rotation;
    res.size = size;
    tek_transform_calc(&res);
    return res;
}

void
tek_transform_init(TekTransform *transform, float x, float y, float z, float rx, float ry, float rz, float sx, float sy,
                   float sz)
{
    transform->position = vec3_create(x, y, z);
    transform->rotation = vec3_create(rx, ry, rz);
    transform->size = vec3_create(sx, sy, sz);
    tek_transform_calc(transform);
}

void tek_transform_calc(TekTransform *transform)
{
    Mat4 trans = mat4_translate(transform->position.x, transform->position.y, transform->position.z);
    Mat4 rot_x = mat4_rotate(transform->rotation.x, 1, 0, 0);
    Mat4 rot_y = mat4_rotate(transform->rotation.y, 0, 1, 0);
    Mat4 rot_z = mat4_rotate(transform->rotation.z, 0, 0, 1);
    Mat4 rot = mat4_mul3(&rot_y, &rot_x, &rot_z);
    Mat4 scale = mat4_scale(transform->size.x, transform->size.y, transform->size.z);

    transform->matrix = mat4_mul3(&trans, &rot, &scale);
}

