#ifndef _TEK_VERTEX_HPP
#define _TEK_VERTEX_HPP

#include "../math/tek_math.hpp"

typedef struct
{
    Vec3 position;
    Vec2 uv;
    Vec3 normal;
}TekMeshVertexData;

TekMeshVertexData tek_mesh_vertex_create(Vec3 position, Vec2 uv, Vec3 normal);

#endif
