#include "tek_transform.hpp"

void tek_transform_init(TekTransform* transform)
{
    transform->position = vec3_create(0,0,0);
    transform->rotation = vec3_create(0,0,0);
    transform->size = vec3_create(1,1,1);
    tek_transform_calc(transform);
}

void tek_transform_calc(TekTransform* transform)
{
    Mat4 trans = mat4_translate(transform->position.x, transform->position.y, transform->position.z);
    Mat4 rot_x = mat4_rotate(transform->rotation.x, 1, 0, 0);
    Mat4 rot_y = mat4_rotate(transform->rotation.y, 0, 1, 0);
    Mat4 rot_z = mat4_rotate(transform->rotation.z, 0, 0, 1);
    Mat4 rot = mat4_mul3(&rot_y, &rot_x, &rot_z);
    Mat4 scale = mat4_scale(transform->size.x, transform->size.y, transform->size.z);

    transform->matrix = mat4_mul3(&trans, &rot, &scale);
}
