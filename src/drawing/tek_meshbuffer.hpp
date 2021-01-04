#ifndef _TEK_MESH_BUFFER_HPP
#define _TEK_MESH_BUFFER_HPP

#include "tek_mesh.hpp"
#include "tek_billboard.hpp"
#include "tek_material.hpp"
#include "tek_camera.hpp"
#include "tek_light.hpp"
#include "../math/tek_transform.hpp"

struct TekMBVertexData
{
	Vec3 position;
	Vec2 uv;
	Vec3 normal;
	Vec4 model0;
	Vec4 model1;
	Vec4 model2;
	Vec4 model3;
	Vec4 flags;
};

TekMBVertexData tek_mb_vertex_create(float x, float y, float z, float u, float v, float nx, float ny, float nz);

typedef enum
{
	TEK_FRONT_TO_BACK,
	TEK_BACK_TO_FRONT
}TekOrderType;

struct DrawCommand
{
	TekMBVertexData vertices[3];
	u32 mat_id;
	u32 tex_id;
	Mat4 transform;
	float distance;
};

struct MaterialCommand
{
	TekMaterial* material;
	u32 tex_id;
};

struct TekMeshbuffer
{
	TekCamera* cam;
	TekLightManager* lights;
	DrawCommand* commands;
	u32 num_commands;
	u32 command_capacity;
	TekOrderType sort_order;
	MaterialCommand material_slots[8];
	u32 num_material_slots;
	TekTexture* texture_slots[8];
	u32 num_texture_slots;

	TekMBVertexData* vertex_buffer;
	u32 vertex_buffer_size;
	u32 vertex_buffer_capacity;
};

void tek_mb_init(TekMeshbuffer* mb,TekCamera* cam, TekLightManager* lights, TekOrderType order);
void tek_mb_destroy(TekMeshbuffer* mb);
void tek_mb_draw_mesh(TekMeshbuffer* mb, TekMesh* mesh, TekMaterial* material, TekTransform* transform);
void tek_mb_draw_vertices(TekMeshbuffer* mb, TekMeshVertexData* vertices, u32 num_vertices, TekMaterial* material);
void tek_mb_draw_billboard(TekMeshbuffer* mb, TekBillboard* bb, TekMaterial* material, TekTransform* transform, bool spherical);
void tek_mb_render(TekMeshbuffer* mb);
void tek_mb_reset(TekMeshbuffer* mb);

#endif
