#ifndef _TEK_MAP_HPP
#define _TEK_MAP_HPP

#include "../drawing/tek_mesh.hpp"
#include "../drawing/tek_material.hpp"
#include "../math/tek_transform.hpp"
#include "../drawing/tek_meshbuffer.hpp"
#include "../drawing/tek_shapebuffer.hpp"

#include <vector>

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
	int bb_id;
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

	TekMaterial* mat;
	TekMeshVertexData vertices[6];

	Vec2 uv0;
	Vec2 uv1;
	Vec2 uv2;
	Vec2 uv3;
};
void tek_plane_move_uv(TekPlane* plane, Vec2 v);
void tek_plane_scale_uv(TekPlane* plane,Vec2 v);
void tek_plane_reset_uv(TekPlane* plane);

class TekBrush
{
public:
	TekBrush();
	void render(TekMeshbuffer* buffer);
	void render_shape(TekShapebuffer* buffer, int mode, int selected);
	void set_material(TekMaterial* mat, int plane);
	void move(float x, float y, float z);
	void move_point(int point, float x, float y, float z);
	void move_line(int line, float x, float y, float z);
	void move_plane(int plane, float x, float y, float z);

	void update_vertices();
	Vec3 points[8];
	TekLine lines[12];
	TekPlane planes[6];
};

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
void tek_quad_render(TekQuad* quad, TekMeshbuffer* buffer);
void tek_quad_render_shape(TekQuad* quad, TekShapebuffer* buffer, int mode, int selected);
void tek_quad_set_material(TekQuad* quad, TekMaterial* mat);
void tek_quad_move(TekQuad* quad, float x, float y, float z);
void tek_quad_move_point(TekQuad* quad, int point, float x, float y, float z);
void tek_quad_move_line(TekQuad* quad, int line, float x, float y, float z);
void tek_quad_move_uv(TekQuad* quad, TekPlane* plane, Vec2 v);
void tek_quad_scale_uv(TekQuad* quad, TekPlane* plane,Vec2 v);
void tek_quad_reset_uv(TekQuad* quad, TekPlane* plane);

struct TekMapMesh
{
	TekMesh mesh;
	char name[128];
};

struct TekMapMaterial
{
	TekMaterial material;
	char name[128];
};

struct TekMap
{
	TekMapMesh* meshes;
	u32 num_meshes;

	TekMapMaterial* materials;
	u32 num_materials;

	std::vector<TekBillboard*> bbs;

	TekSky* sky;

	TekQuad* quads;
	u32 num_quads;

	std::vector<TekBrush> brushes;

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

#endif
