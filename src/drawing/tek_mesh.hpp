#ifndef _TEK_MESH_HPP
#define _TEK_MESH_HPP

#include "../core/tek_core.hpp"
#include "../math/tek_math.hpp"
#include "tek_vertex.hpp"

typedef struct
{
    u32 vao;
    u32 vbo;
    u32 ibo;
    u32 num_vertices;
    u32 num_indices;
}TekMesh;

void tek_mesh_destroy(TekMesh* mesh);
void tek_mesh_render(TekMesh* mesh);
void tek_mesh_create(TekMesh* mesh, TekMeshVertexData* vertices, u32 num_vertices, u32* indices, u32 num_indices);

#endif
