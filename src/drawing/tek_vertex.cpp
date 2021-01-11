#include "tek_vertex.hpp"

TekMeshVertexData tek_mesh_vertex_create(Vec3 position, Vec2 uv, Vec3 normal)
{
    TekMeshVertexData res;
    res.position = position;
    res.uv = uv;
    res.normal = normal;
    return res;
}
