#ifndef _TEK_MAP_HPP
#define _TEK_MAP_HPP

#include "../drawing/tek_mesh.hpp"
#include "../drawing/tek_material.hpp"
#include "../math/tek_transform.hpp"
#include "../drawing/tek_meshbuffer.hpp"
#include "../drawing/tek_shapebuffer.hpp"
#include "tek_assets.hpp"

struct TekMap;

enum TekMapObjectType
{
	TEK_MAP_OBJECT_STATIC,
	TEK_MAP_OBJECT_BILLBOARD,
};

struct TekStaticObj
{
	int mesh_id;
	int mat_id;
};

struct TekBillboardObj
{
	TekBillboard bb;
	int mat_id;
};

struct TekSky
{
	TekMesh* skybox;
	TekMaterial* sky_material;
};

struct TekMapObject
{
	TekMapObjectType type;
	TekStaticObj* static_obj;
	TekBillboardObj* bb_obj;

	TekTransform transform;
};

void tek_object_render_shape(TekMapObject* obj, TekShapebuffer* buffer, bool selected);

struct TekLine
{
	int p0;
	int p1;
};

struct TekPlane
{
	int p0;
	int p1;
	int p2;
	int p3;

	int l0;
	int l1;
	int l2;
	int l3;

	int mat_id;
	TekMeshVertexData vertices[6];

	Vec2 uv0;
	Vec2 uv1;
	Vec2 uv2;
	Vec2 uv3;
};
void tek_plane_move_uv(TekPlane* plane, Vec2 v);
void tek_plane_scale_uv(TekPlane* plane,Vec2 v);
void tek_plane_reset_uv(TekPlane* plane);

struct TekBrush
{	
	Vec3 points[8];
	TekLine lines[12];
	TekPlane planes[6];
};


void tek_brush_init(TekBrush* brush);
void tek_brush_render(TekBrush* brush, TekMeshbuffer* buffer, TekMap* map);
void tek_brush_render_shape(TekBrush* brush, TekShapebuffer* buffer, int mode, int selected);
void tek_brush_move(TekBrush* brush, float x, float y, float z);
void tek_brush_move_point(TekBrush* brush, int point, float x, float y, float z);
void tek_brush_move_line(TekBrush* brush, int line, float x, float y, float z);
void tek_brush_move_plane(TekBrush* brush, int plane, float x, float y, float z);

void tek_brush_move_uv(TekBrush* brush, TekPlane* plane, Vec2 v);
void tek_brush_scale_uv(TekBrush* brush, TekPlane* plane,Vec2 v);
void tek_brush_reset_uv(TekBrush* brush, TekPlane* plane);

struct TekQuad
{
	Vec3 points[4];
	TekLine lines[4];
	TekPlane plane;
};
void tek_quad_init(TekQuad* quad);
void tek_quad_render(TekQuad* quad, TekMeshbuffer* buffer, TekMap* map);
void tek_quad_render_shape(TekQuad* quad, TekShapebuffer* buffer, int mode, int selected);
void tek_quad_move(TekQuad* quad, float x, float y, float z);
void tek_quad_move_point(TekQuad* quad, int point, float x, float y, float z);
void tek_quad_move_line(TekQuad* quad, int line, float x, float y, float z);
void tek_quad_move_uv(TekQuad* quad, TekPlane* plane, Vec2 v);
void tek_quad_scale_uv(TekQuad* quad, TekPlane* plane,Vec2 v);
void tek_quad_reset_uv(TekQuad* quad, TekPlane* plane);

struct TekMap
{
	TekAssetMesh* meshes;
	u32 num_meshes;

	TekAssetMaterial* materials;
	u32 num_materials;

	TekSky* sky;

	TekQuad* quads;
	u32 num_quads;

	TekBrush* brushes;
	u32 num_brushes;

	TekMapObject* objects;
	u32 num_objects;
};

bool tek_map_init(TekMap* map);
void tek_map_destroy(TekMap* map);
void tek_map_render(TekMap* map, TekMeshbuffer* buffer);
void tek_map_update(TekMap* map,float delta);
bool tek_map_save(TekMap* map, const char* name);
bool tek_map_load(TekMap* map, const char* name);

TekBrush* tek_map_add_brush(TekMap* map);
TekQuad* tek_map_add_quad(TekMap* map);
void tek_map_add_material(TekMap* map, TekAssetMaterial mat);
void tek_map_add_mesh(TekMap* map, TekAssetMesh mesh);
void tek_map_brush_set_material(TekMap* map, TekBrush* brush, TekAssetMaterial* mat, int plane);
void tek_map_quad_set_material(TekMap* map, TekQuad* quad, TekAssetMaterial* mat);
TekMapObject* tek_map_add_static_object(TekMap* map);
TekMapObject* tek_map_add_bb_object(TekMap* map);
void tek_map_object_set_material(TekMap* map, TekMapObject* obj, TekAssetMaterial* mat);
void tek_map_object_set_mesh(TekMap* map, TekMapObject* obj, TekAssetMesh* mesh);

#endif
