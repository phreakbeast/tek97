#ifndef _TEK_SKELETAL_MESH_HPP
#define _TEK_SKELETAL_MESH_HPP

#include "../core/tek_core.hpp"
#include "../math/tek_math.hpp"

typedef struct
{
	Vec3 position;
	Vec2 uv;
	Vec3 normal;
	Vec4 color;
	Vec4 bones;
	Vec4 weights;
} TekSkeletalVertexData;

typedef struct
{
	u32 vao;
	u32 vbo;
	u32 ibo;

	TekSkeletalVertexData *vertices;
	u32 num_vertices;
	u32 *indices;
	u32 num_indices;
} TekSkeletalMesh;


void tek_skeletal_mesh_destroy(TekSkeletalMesh *mesh);

bool tek_skeletal_mesh_load(TekSkeletalMesh *mesh, const char *filename);

#endif