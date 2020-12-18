#ifndef _TEK_MESH_HPP
#define _TEK_MESH_HPP

#include "../core/tek_core.hpp"
#include "../math/tek_math.hpp"
#include "tek_shader.hpp"

typedef struct
{
    Vec3 position;
    Vec2 uv;
    Vec3 normal;
}TekMeshVertexData;

TekMeshVertexData tek_mesh_vertex_data_create(float x, float y, float z, float u, float v, float nx, float ny, float nz);

typedef struct
{
    Vec3 min;
    Vec3 max;
    Vec3 center;

    u32 vao;
    u32 vbo;
    u32 ibo;

    u32 num_vertices;
    u32 num_indices;
    TekMeshVertexData *vertices;
    u32 *indices;
}TekMesh;

void tek_mesh_destroy(TekMesh* mesh);

void tek_mesh_create(TekMesh* mesh, TekMeshVertexData *vertices, u32 num_vertices, u32 *indices, u32 num_indices, bool calc_normals);

void tek_mesh_render(TekMesh* mesh);

bool tek_mesh_load(TekMesh* mesh, const char *filename);

void tek_mesh_create_plane(TekMesh* mesh, float width, float depth, int x_segments, int y_segments);

void tek_mesh_create_box(TekMesh* mesh, float width, float height, float depth);

void tek_mesh_create_skybox(TekMesh* mesh, float width);

void tek_mesh_create_cylinder(TekMesh* mesh, float rad, int segment, float h, float uv_x, float uv_y);

void tek_mesh_create_inner_cylinder(TekMesh* mesh, float rad, int segment, float h, float uv_x, float uv_y, int caps);

void tek_mesh_create_sphere(TekMesh* mesh, float rad, int rings, int sectors, int inverted);


#endif